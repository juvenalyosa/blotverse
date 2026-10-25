#include "app_controller.hpp"

#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QTimer>
#include <QtConcurrent>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <numeric>

#include "blotverse/imaging/detect.hpp"
#include "blotverse/report/report.hpp"
#include "blotverse/sim/simulator.hpp"

using namespace blotverse;

namespace {

// Stages reported by analyze(), with their share of the total work.
struct StageInfo {
  const char* name;
  double weight;
};
constexpr StageInfo kStages[] = {
    {"Quality control", 0.05}, {"Quantification", 0.20}, {"Universes", 0.15}, {"Joint test", 0.55}, {"Done", 0.05}};

int stage_index_of(const QString& stage) {
  for (int i = 0; i < static_cast<int>(std::size(kStages)); ++i)
    if (stage == QLatin1String(kStages[i].name)) return i;
  return 0;
}

QVariant number_or_null(double v) { return std::isfinite(v) ? QVariant(v) : QVariant(); }

QStringList to_qstrings(const std::vector<std::string>& v) {
  QStringList out;
  for (const auto& s : v) out << QString::fromStdString(s);
  return out;
}

// 16-bit acquisition -> 8-bit display: stretch between the 1st and 99.7th
// percentiles and invert, so bands look dark on a light membrane.
QImage display_image(const ImageU16& px) {
  QImage img(px.width(), px.height(), QImage::Format_Grayscale8);
  if (px.empty()) return img;
  std::vector<std::uint16_t> sample;
  const std::size_t step = std::max<std::size_t>(1, px.size() / 200000);
  for (std::size_t i = 0; i < px.size(); i += step) sample.push_back(px.data()[i]);
  auto at = [&](double q) {
    auto k = static_cast<std::ptrdiff_t>(q * static_cast<double>(sample.size() - 1));
    std::nth_element(sample.begin(), sample.begin() + k, sample.end());
    return static_cast<double>(sample[static_cast<std::size_t>(k)]);
  };
  const double lo = at(0.01), hi = std::max(at(0.997), lo + 1.0);
  for (int y = 0; y < px.height(); ++y) {
    uchar* line = img.scanLine(y);
    for (int x = 0; x < px.width(); ++x) {
      const double t = std::clamp((px(x, y) - lo) / (hi - lo), 0.0, 1.0);
      line[x] = static_cast<uchar>(std::lround(255.0 * (1.0 - std::sqrt(t))));
    }
  }
  return img;
}

}  // namespace

AppController::AppController(QObject* parent) : QObject(parent) {
  QFile file(QStringLiteral(":/spec/multiverse.yaml"));
  if (!file.open(QIODevice::ReadOnly)) qFatal("BlotVerse: embedded multiverse specification is missing");
  spec_ = parse_spec(file.readAll().toStdString());
  connect(&watcher_, &QFutureWatcher<RunOutcome>::finished, this, &AppController::onRunFinished);
}

AppController::~AppController() { watcher_.waitForFinished(); }

QString AppController::experimentName() const {
  return experiment_ ? QString::fromStdString(experiment_->name) : QString();
}

QString AppController::specSummary() const {
  const ExperimentFacts with_tp{true, true}, without_tp{false, true};
  return tr("%1 v%2 · %3 universes (%4 without total protein) · default %5")
      .arg(QString::fromStdString(spec_.name), QString::fromStdString(spec_.version))
      .arg(plan_universes(spec_, with_tp).universes.size())
      .arg(plan_universes(spec_, without_tp).universes.size())
      .arg(QString::fromStdString(spec_.describe(spec_.default_universe)));
}

int AppController::universeTotal() const {
  if (result_) return static_cast<int>(result_->multiverse.plan.universes.size());
  if (!experiment_) return 0;
  return static_cast<int>(plan_universes(spec_, {experiment_->has_total_protein(), true}).universes.size());
}

double AppController::overallFraction() const {
  double done = 0.0;
  for (int i = 0; i < stage_index_; ++i) done += kStages[i].weight;
  return std::min(1.0, done + kStages[stage_index_].weight * stage_fraction_);
}

void AppController::setPhase(const QString& phase, const QString& error) {
  phase_ = phase;
  error_ = error;
  emit phaseChanged();
}

QVariantList AppController::blots() const {
  QVariantList out;
  if (!experiment_) return out;
  for (const auto& b : experiment_->blots) {
    QVariantMap m;
    m[QStringLiteral("id")] = QString::fromStdString(b.id);
    m[QStringLiteral("file")] = QString::fromStdString(b.image_file);
    m[QStringLiteral("width")] = b.image.pixels.width();
    m[QStringLiteral("height")] = b.image.pixels.height();
    m[QStringLiteral("hasTotalProtein")] = b.total_protein.has_value();
    m[QStringLiteral("warnings")] = to_qstrings(b.image.meta.warnings);
    QVariantList lanes;
    for (std::size_t i = 0; i < b.lanes.size(); ++i) {
      QVariantMap lane;
      lane[QStringLiteral("group")] = QString::fromStdString(b.lanes[i].group);
      lane[QStringLiteral("sample")] = QString::fromStdString(b.lanes[i].sample);
      const auto groups = experiment_->groups();
      lane[QStringLiteral("groupIndex")] =
          static_cast<int>(std::find(groups.begin(), groups.end(), b.lanes[i].group) - groups.begin());
      if (b.annotation && i < b.annotation->lane_count()) {
        lane[QStringLiteral("x")] = b.annotation->lanes[i].x_center;
        lane[QStringLiteral("width")] = b.annotation->lanes[i].width;
        lane[QStringLiteral("targetY")] = b.annotation->target_y[i];
        lane[QStringLiteral("referenceY")] = b.annotation->reference_y[i];
      }
      lanes << lane;
    }
    m[QStringLiteral("lanes")] = lanes;
    m[QStringLiteral("annotated")] = b.annotation.has_value();
    m[QStringLiteral("annotationSource")] =
        b.annotation ? QString::fromStdString(b.annotation->source) : QString();
    m[QStringLiteral("halfHeight")] = b.annotation ? b.annotation->band_half_height : 0;
    out << m;
  }
  return out;
}

QImage AppController::blotImage(int blot) const {
  QMutexLocker lock(&images_mutex_);
  return blot >= 0 && blot < images_.size() ? images_[blot] : QImage();
}

void AppController::rebuildImages() {
  QVector<QImage> images;
  if (experiment_)
    for (const auto& b : experiment_->blots) images << display_image(b.image.pixels);
  {
    QMutexLocker lock(&images_mutex_);
    images_ = std::move(images);
  }
  ++image_revision_;
}

void AppController::loadExperiment(Experiment experiment, const QString& path) {
  // Lanes and bands are detected up front so they can be checked and corrected
  // before running; a manual annotation in the file is kept as is.
  for (auto& b : experiment.blots) {
    if (b.annotation) continue;
    try {
      b.annotation = detect_annotation(to_float(b.image.pixels), b.lanes.size(), b.hints);
    } catch (const std::exception& e) {
      b.image.meta.warnings.push_back(std::string("automatic lane detection failed: ") + e.what());
    }
  }
  experiment_ = std::move(experiment);
  experiment_path_ = path;
  result_.reset();
  rebuildImages();
  emit experimentChanged();
  emit resultChanged();
  setPhase(QStringLiteral("setup"));
}

void AppController::openExperiment(const QUrl& file) {
  const QString path = file.isLocalFile() ? file.toLocalFile() : file.toString();
  try {
    loadExperiment(load_experiment(path.toStdString()), path);
  } catch (const std::exception& e) {
    setPhase(QStringLiteral("error"), QString::fromStdString(e.what()));
  }
}

void AppController::simulateDemo(const QString& level, double lfc) {
  try {
    SimParams p = sim_preset(parse_artifact_level(level.toStdString()));
    p.n_gels = 3;
    p.reps_per_gel = 2;
    p.true_lfc = {0.0, lfc};
    p.seed = QRandomGenerator::global()->generate64();
    // Written to disk so the demo has real files, like any experiment
    // (reports and the reproducibility file need them).
    const QString dir = QDir(QDir::tempPath())
                            .filePath(QStringLiteral("blotverse-demo-%1")
                                          .arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd-hhmmss-zzz"))));
    write_simulation(dir.toStdString(), simulate(p));
    const QString path = QDir(dir).filePath(QStringLiteral("experiment.yaml"));
    Experiment ex = load_experiment(path.toStdString());
    ex.name = tr("Demo: %1 artefacts, true log2 FC %2").arg(level).arg(lfc).toStdString();
    loadExperiment(std::move(ex), path);
  } catch (const std::exception& e) {
    setPhase(QStringLiteral("error"), QString::fromStdString(e.what()));
  }
}

void AppController::redetect(int blot) {
  if (!experiment_ || blot < 0 || blot >= static_cast<int>(experiment_->blots.size())) return;
  auto& b = experiment_->blots[static_cast<std::size_t>(blot)];
  try {
    b.annotation = detect_annotation(to_float(b.image.pixels), b.lanes.size(), b.hints);
  } catch (const std::exception& e) {
    setPhase(phase_, QString::fromStdString(e.what()));
  }
  emit experimentChanged();
}

void AppController::moveLane(int blot, int lane, double x) {
  if (!experiment_) return;
  auto& ann = experiment_->blots.at(static_cast<std::size_t>(blot)).annotation;
  if (!ann || lane < 0 || lane >= static_cast<int>(ann->lane_count())) return;
  ann->lanes[static_cast<std::size_t>(lane)].x_center = x;
  ann->source = "manual";
  emit experimentChanged();
}

void AppController::moveBand(int blot, int lane, bool target, double y) {
  if (!experiment_) return;
  auto& ann = experiment_->blots.at(static_cast<std::size_t>(blot)).annotation;
  if (!ann || lane < 0 || lane >= static_cast<int>(ann->lane_count())) return;
  (target ? ann->target_y : ann->reference_y)[static_cast<std::size_t>(lane)] = std::round(y);
  ann->source = "manual";
  emit experimentChanged();
}

void AppController::setBandHalfHeight(int blot, int halfHeight) {
  if (!experiment_) return;
  auto& ann = experiment_->blots.at(static_cast<std::size_t>(blot)).annotation;
  if (!ann) return;
  ann->band_half_height = std::max(1, halfHeight);
  ann->source = "manual";
  emit experimentChanged();
}

void AppController::saveAnnotations() {
  if (!experiment_ || experiment_path_.isEmpty()) return;
  // Written next to the original, which is never overwritten; the saved file
  // is what the analysis and reproducibility file then refer to.
  QFileInfo info(experiment_path_);
  QString target = experiment_path_;
  if (!info.completeBaseName().endsWith(QStringLiteral(".annotated")))
    target = info.dir().filePath(info.completeBaseName() + QStringLiteral(".annotated.yaml"));
  try {
    save_experiment(target.toStdString(), *experiment_);
    Experiment reloaded = load_experiment(target.toStdString());
    reloaded.name = experiment_->name;
    experiment_ = std::move(reloaded);
    experiment_path_ = target;
    emit experimentChanged();
  } catch (const std::exception& e) {
    setPhase(phase_, QString::fromStdString(e.what()));
  }
}

void AppController::onProgress(const QString& stage, double fraction) {
  stage_ = stage;
  stage_index_ = stage_index_of(stage);
  stage_fraction_ = fraction;
  emit progressChanged();
}

void AppController::run() {
  if (!experiment_ || watcher_.isRunning()) return;
  stage_ = QStringLiteral("Quality control");
  stage_index_ = 0;
  stage_fraction_ = 0.0;
  last_progress_permille_ = -1;
  emit progressChanged();
  run_clock_.start();
  setPhase(QStringLiteral("running"));

  const Experiment experiment = *experiment_;
  const MultiverseSpec spec = spec_;
  watcher_.setFuture(QtConcurrent::run([this, experiment, spec]() -> RunOutcome {
    AnalysisOptions options;
    options.progress = [this](const std::string& stage, double fraction) {
      // Throttle: forward a stage change or each 0.5 % step to the GUI thread.
      const int permille = stage_index_of(QString::fromStdString(stage)) * 1000 + static_cast<int>(fraction * 200);
      if (permille == last_progress_permille_.exchange(permille)) return;
      QMetaObject::invokeMethod(
          this, [this, s = QString::fromStdString(stage), fraction] { onProgress(s, fraction); },
          Qt::QueuedConnection);
    };
    try {
      return {std::make_shared<AnalysisResult>(analyze(experiment, spec, options)), {}};
    } catch (const std::exception& e) {
      return {nullptr, QString::fromStdString(e.what())};
    }
  }));
}

void AppController::onRunFinished() {
  // Fast analyses still show the progress screen briefly, so the transition
  // reads as "done" instead of a flicker.
  const qint64 remaining = min_running_ms_ - run_clock_.elapsed();
  if (remaining > 0) {
    QTimer::singleShot(static_cast<int>(remaining), this, &AppController::onRunFinished);
    return;
  }
  const RunOutcome outcome = watcher_.result();
  if (!outcome.result) {
    setPhase(QStringLiteral("error"), outcome.error);
    return;
  }
  result_ = outcome.result;
  contrast_index_ = 0;
  emit resultChanged();
  emit experimentChanged();
  setPhase(QStringLiteral("results"));
}

void AppController::waitForRun() { watcher_.waitForFinished(); }

QStringList AppController::contrasts() const {
  QStringList out;
  if (result_)
    for (const auto& s : result_->summaries) out << QString::fromStdString(s.contrast.label);
  return out;
}

void AppController::setContrastIndex(int index) {
  if (!result_ || index < 0 || index >= static_cast<int>(result_->summaries.size()) || index == contrast_index_)
    return;
  contrast_index_ = index;
  emit resultChanged();
}

QVariantMap AppController::result() const {
  QVariantMap m;
  if (!result_ || result_->summaries.empty()) return m;
  const auto c = static_cast<std::size_t>(contrast_index_);
  const auto& s = result_->summaries[c];
  const auto& mv = result_->multiverse;

  m[QStringLiteral("contrast")] = QString::fromStdString(s.contrast.label);
  m[QStringLiteral("verdict")] = QString::fromLatin1(verdict_label(s.verdict));
  m[QStringLiteral("verdictKey")] = static_cast<int>(s.verdict);
  // The rule that produced the verdict, in words (thresholds from the spec).
  const auto& v = spec_.verdict;
  const QString r_text = QString::number(s.robustness, 'f', 3);
  const QString p_text = QString::number(s.joint.p, 'f', 4);
  QString reason;
  switch (s.verdict) {
    case Verdict::Robust:
      reason = tr("R = %1 ≥ %2 and the joint permutation test is significant (p = %3 < %4).")
                   .arg(r_text).arg(v.robust_min_r).arg(p_text).arg(v.joint_alpha);
      break;
    case Verdict::Fragile:
      reason = tr("The joint test is significant (p = %1), but R = %2 is below %3: part of the defensible analyses disagree.")
                   .arg(p_text, r_text).arg(v.robust_min_r);
      break;
    case Verdict::NotSupported:
      reason = s.joint.p >= v.joint_alpha
                   ? tr("The joint permutation test is not significant (p = %1 ≥ %2), whatever R (= %3).")
                         .arg(p_text).arg(v.joint_alpha).arg(r_text)
                   : tr("R = %1 is below %2.").arg(r_text).arg(v.fragile_min_r);
      break;
    case Verdict::NotEvaluable:
      reason = tr("The design has too few lanes per blot for the joint test to reach %1.").arg(v.joint_alpha);
      break;
  }
  m[QStringLiteral("verdictReason")] = reason;
  m[QStringLiteral("robustness")] = number_or_null(s.robustness);
  m[QStringLiteral("jointP")] = number_or_null(s.joint.p);
  m[QStringLiteral("jointExact")] = s.joint.exact;
  m[QStringLiteral("defaultLfc")] = number_or_null(s.default_lfc);
  m[QStringLiteral("defaultP")] = number_or_null(s.default_p);
  m[QStringLiteral("medianLfc")] = number_or_null(s.median_lfc);
  m[QStringLiteral("lfcLow")] = number_or_null(s.lfc_q05);
  m[QStringLiteral("lfcHigh")] = number_or_null(s.lfc_q95);
  m[QStringLiteral("opposite")] = number_or_null(s.opposite_fraction);
  m[QStringLiteral("significant")] = number_or_null(s.significant_fraction);
  m[QStringLiteral("nValid")] = static_cast<int>(s.n_valid);
  m[QStringLiteral("nUniverses")] = static_cast<int>(s.n_universes);
  m[QStringLiteral("nExcluded")] = static_cast<int>(s.n_excluded);
  m[QStringLiteral("seconds")] = result_->seconds;
  m[QStringLiteral("permutations")] = static_cast<int>(result_->joint_permutations);
  m[QStringLiteral("notes")] = to_qstrings(s.notes);
  m[QStringLiteral("warnings")] = to_qstrings(result_->warnings);
  m[QStringLiteral("mostInfluential")] =
      s.most_influential ? QString::fromLatin1(decision_label(*s.most_influential)) : QString();

  QVariantList influence;
  for (Decision d : kDecisions) {
    QVariantMap bar;
    bar[QStringLiteral("label")] = QString::fromLatin1(decision_label(d));
    bar[QStringLiteral("value")] = s.eta2[static_cast<std::size_t>(d)];
    bar[QStringLiteral("top")] = s.most_influential == d;
    influence << bar;
  }
  m[QStringLiteral("influence")] = influence;

  // Specification curve: valid universes ordered by effect (never by p).
  const auto& outcomes = mv.outcomes[c];
  std::vector<std::size_t> order;
  for (std::size_t i = 0; i < outcomes.size(); ++i)
    if (outcomes[i].valid) order.push_back(i);
  std::stable_sort(order.begin(), order.end(), [&](auto a, auto b) { return outcomes[a].lfc < outcomes[b].lfc; });
  const int default_sign = (s.default_lfc > 0) - (s.default_lfc < 0);
  QVariantList curve;
  for (std::size_t i : order) {
    const auto& o = outcomes[i];
    QVariantMap pt;
    const int sign = (o.lfc > 0) - (o.lfc < 0);
    pt[QStringLiteral("lfc")] = o.lfc;
    pt[QStringLiteral("p")] = o.p;
    pt[QStringLiteral("kind")] = !(o.p < spec_.alpha) ? 0 : (sign == default_sign ? 1 : 2);
    pt[QStringLiteral("isDefault")] = mv.plan.default_index == i;
    pt[QStringLiteral("label")] = QString::fromStdString(spec_.describe(mv.plan.universes[i]));
    curve << pt;
  }
  m[QStringLiteral("curve")] = curve;

  // Branch panel: for each decision and branch, the curve positions using it.
  QVariantList panel;
  for (std::size_t d = 0; d < kNumDecisions; ++d) {
    QVariantMap row;
    row[QStringLiteral("decision")] = QString::fromLatin1(decision_label(kDecisions[d]));
    QVariantList branches;
    for (std::size_t b = 0; b < spec_.decisions[d].branches.size(); ++b) {
      QVariantList ranks;
      for (std::size_t r = 0; r < order.size(); ++r)
        if (mv.plan.universes[order[r]][d] == b) ranks << static_cast<int>(r);
      QVariantMap br;
      br[QStringLiteral("id")] = QString::fromStdString(spec_.decisions[d].branches[b].id);
      br[QStringLiteral("ranks")] = ranks;
      branches << br;
    }
    row[QStringLiteral("branches")] = branches;
    panel << row;
  }
  m[QStringLiteral("panel")] = panel;
  return m;
}

void AppController::exportResults(const QUrl& folder) {
  if (!result_ || !experiment_) return;
  const QString dir = folder.isLocalFile() ? folder.toLocalFile() : folder.toString();
  try {
    const std::filesystem::path out = dir.toStdString();
    std::filesystem::create_directories(out);
    std::ofstream(out / "report.html") << render_html_report(spec_, *experiment_, *result_);
    write_universes_csv(out / "universes.csv", spec_, *result_);
    write_summary_csv(out / "summary.csv", *result_);
    write_lanes_csv(out / "lanes.csv", spec_, *experiment_, *result_);
    write_repro_file(out / "repro.yaml", spec_, *experiment_,
                     QFileInfo(experiment_path_).absolutePath().toStdString(), *result_);
    last_export_ = QDir(dir).filePath(QStringLiteral("report.html"));
    emit exported();
  } catch (const std::exception& e) {
    setPhase(phase_, QString::fromStdString(e.what()));
  }
}

void AppController::openLastReport() {
  if (!last_export_.isEmpty()) QDesktopServices::openUrl(QUrl::fromLocalFile(last_export_));
}

void AppController::backToSetup() {
  if (experiment_) setPhase(QStringLiteral("setup"));
}

void AppController::reset() {
  experiment_.reset();
  result_.reset();
  experiment_path_.clear();
  rebuildImages();
  emit experimentChanged();
  emit resultChanged();
  setPhase(QStringLiteral("start"));
}
