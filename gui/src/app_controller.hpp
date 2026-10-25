#pragma once

#include <QElapsedTimer>
#include <QFutureWatcher>
#include <QImage>
#include <QMutex>
#include <QObject>
#include <QStringList>
#include <QUrl>
#include <QVariantList>

#include <atomic>
#include <memory>
#include <optional>

#include "blotverse/analysis/spec.hpp"
#include "blotverse/experiment.hpp"
#include "blotverse/pipeline.hpp"

// Outcome of a background analysis; errors are carried as text because
// QtConcurrent only transports QException subclasses.
struct RunOutcome {
  std::shared_ptr<blotverse::AnalysisResult> result;
  QString error;
};

// Bridge between the QML interface and the BlotVerse library. The interface
// holds no analysis state of its own: the experiment (with any lane/band
// corrections) and the results live here and are written back to the same
// YAML, report and reproducibility files the command line uses.
class AppController : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString phase READ phase NOTIFY phaseChanged)
  Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY phaseChanged)
  Q_PROPERTY(QString experimentName READ experimentName NOTIFY experimentChanged)
  Q_PROPERTY(QString experimentPath READ experimentPath NOTIFY experimentChanged)
  Q_PROPERTY(QVariantList blots READ blots NOTIFY experimentChanged)
  Q_PROPERTY(int imageRevision READ imageRevision NOTIFY experimentChanged)
  Q_PROPERTY(QString specSummary READ specSummary CONSTANT)

  Q_PROPERTY(QString stage READ stage NOTIFY progressChanged)
  Q_PROPERTY(int stageIndex READ stageIndex NOTIFY progressChanged)
  Q_PROPERTY(double stageFraction READ stageFraction NOTIFY progressChanged)
  Q_PROPERTY(double overallFraction READ overallFraction NOTIFY progressChanged)
  Q_PROPERTY(int universeTotal READ universeTotal NOTIFY experimentChanged)

  Q_PROPERTY(QVariantMap result READ result NOTIFY resultChanged)
  Q_PROPERTY(QStringList contrasts READ contrasts NOTIFY resultChanged)
  Q_PROPERTY(int contrastIndex READ contrastIndex WRITE setContrastIndex NOTIFY resultChanged)
  Q_PROPERTY(QString lastExport READ lastExport NOTIFY exported)

 public:
  explicit AppController(QObject* parent = nullptr);
  ~AppController() override;

  QString phase() const { return phase_; }
  QString errorMessage() const { return error_; }
  QString experimentName() const;
  QString experimentPath() const { return experiment_path_; }
  QVariantList blots() const;
  int imageRevision() const { return image_revision_; }
  QString specSummary() const;

  QString stage() const { return stage_; }
  int stageIndex() const { return stage_index_; }
  double stageFraction() const { return stage_fraction_; }
  double overallFraction() const;
  int universeTotal() const;

  QVariantMap result() const;
  QStringList contrasts() const;
  int contrastIndex() const { return contrast_index_; }
  void setContrastIndex(int index);
  QString lastExport() const { return last_export_; }

  // Display image of a blot (contrast-stretched, dark bands on light).
  QImage blotImage(int blot) const;

  Q_INVOKABLE void openExperiment(const QUrl& file);
  Q_INVOKABLE void simulateDemo(const QString& level, double lfc);
  Q_INVOKABLE void redetect(int blot);
  Q_INVOKABLE void moveLane(int blot, int lane, double x);
  Q_INVOKABLE void moveBand(int blot, int lane, bool target, double y);
  Q_INVOKABLE void setBandHalfHeight(int blot, int halfHeight);
  Q_INVOKABLE void saveAnnotations();
  Q_INVOKABLE void run();
  Q_INVOKABLE void exportResults(const QUrl& folder);
  Q_INVOKABLE void openLastReport();
  Q_INVOKABLE void backToSetup();
  Q_INVOKABLE void reset();

  // Blocks until a running analysis has finished (tests).
  void waitForRun();

 signals:
  void phaseChanged();
  void experimentChanged();
  void progressChanged();
  void resultChanged();
  void exported();

 private:
  void setPhase(const QString& phase, const QString& error = {});
  void loadExperiment(blotverse::Experiment experiment, const QString& path);
  void rebuildImages();
  void onProgress(const QString& stage, double fraction);
  void onRunFinished();

  blotverse::MultiverseSpec spec_;
  std::optional<blotverse::Experiment> experiment_;
  std::shared_ptr<blotverse::AnalysisResult> result_;
  QString experiment_path_;
  QString phase_ = QStringLiteral("start");
  QString error_;
  QVector<QImage> images_;
  mutable QMutex images_mutex_;
  int image_revision_ = 0;

  QString stage_;
  int stage_index_ = 0;
  double stage_fraction_ = 0.0;
  std::atomic<int> last_progress_permille_{-1};
  int contrast_index_ = 0;
  QString last_export_;
  QFutureWatcher<RunOutcome> watcher_;
  QElapsedTimer run_clock_;
  int min_running_ms_ = 1800;  // let the progress animation be seen on fast runs

 public:
  void setMinimumRunningTime(int ms) { min_running_ms_ = ms; }
};
