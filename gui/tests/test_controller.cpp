// Headless test of the GUI controller (QT_QPA_PLATFORM=offscreen).

#include <QGuiApplication>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>

#include "app_controller.hpp"

class ControllerTest : public QObject {
  Q_OBJECT
 private slots:
  void demoRunsEndToEnd() {
    AppController app;
    app.setMinimumRunningTime(0);
    QCOMPARE(app.phase(), QStringLiteral("start"));
    QVERIFY(app.specSummary().contains(QStringLiteral("432")));

    app.simulateDemo(QStringLiteral("clean"), 1.0);
    QCOMPARE(app.phase(), QStringLiteral("setup"));
    QCOMPARE(app.blots().size(), 3);
    QVERIFY(app.blots().front().toMap().value(QStringLiteral("annotated")).toBool());
    QVERIFY(!app.blotImage(0).isNull());

    QSignalSpy progress(&app, &AppController::progressChanged);
    QSignalSpy done(&app, &AppController::resultChanged);
    app.run();
    QCOMPARE(app.phase(), QStringLiteral("running"));
    QVERIFY(done.wait(120000));
    QCOMPARE(app.phase(), QStringLiteral("results"));
    QVERIFY(progress.count() > 3);
    QCOMPARE(app.overallFraction(), 1.0);

    const QVariantMap r = app.result();
    QCOMPARE(r.value(QStringLiteral("nUniverses")).toInt(), 432);
    QCOMPARE(r.value(QStringLiteral("curve")).toList().size(), r.value(QStringLiteral("nValid")).toInt());
    QCOMPARE(r.value(QStringLiteral("influence")).toList().size(), 6);
    QVERIFY(r.value(QStringLiteral("robustness")).toDouble() > 0.5);

    QTemporaryDir out;
    app.exportResults(QUrl::fromLocalFile(out.path()));
    QVERIFY(QFile::exists(out.filePath(QStringLiteral("report.html"))));
    QVERIFY(QFile::exists(out.filePath(QStringLiteral("repro.yaml"))));
  }

  void laneCorrectionsAreKept() {
    AppController app;
    app.simulateDemo(QStringLiteral("clean"), 0.5);
    const double x0 = app.blots().front().toMap()[QStringLiteral("lanes")].toList().front().toMap()[QStringLiteral("x")].toDouble();
    app.moveLane(0, 0, x0 + 3);
    app.moveBand(0, 0, true, 100);
    const QVariantMap b = app.blots().front().toMap();
    QCOMPARE(b[QStringLiteral("annotationSource")].toString(), QStringLiteral("manual"));
    QCOMPARE(b[QStringLiteral("lanes")].toList().front().toMap()[QStringLiteral("x")].toDouble(), x0 + 3);
    app.saveAnnotations();
    QVERIFY(app.experimentPath().endsWith(QStringLiteral(".annotated.yaml")));
    QCOMPARE(app.blots().front().toMap()[QStringLiteral("annotationSource")].toString(), QStringLiteral("manual"));
  }
};

QTEST_MAIN(ControllerTest)
#include "test_controller.moc"
