// BlotVerse desktop application.

#include <QDir>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QTimer>
#include <QIcon>
#include <functional>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "app_controller.hpp"
#include "blot_image_provider.hpp"
#include "blotverse/version.hpp"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);
  QGuiApplication::setApplicationName(QStringLiteral("BlotVerse"));
  QGuiApplication::setOrganizationName(QStringLiteral("BlotVerse"));
  QGuiApplication::setApplicationVersion(QString::fromLatin1(blotverse::kVersion));
  QQuickStyle::setStyle(QStringLiteral("Basic"));

  AppController controller;
  QQmlApplicationEngine engine;
  engine.addImageProvider(QStringLiteral("blot"), new BlotImageProvider(&controller));
  engine.rootContext()->setContextProperty(QStringLiteral("app"), &controller);
  engine.rootContext()->setContextProperty(QStringLiteral("appVersion"),
                                           QString::fromLatin1(blotverse::kVersion));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
                   [] { QCoreApplication::exit(1); }, Qt::QueuedConnection);
  engine.loadFromModule(QStringLiteral("BlotVerse"), QStringLiteral("Main"));

  // --capture DIR: walk through the demo and save a screenshot of every
  // screen (documentation and visual checks).
  const int capture = QCoreApplication::arguments().indexOf(QStringLiteral("--capture"));
  if (capture > 0 && capture + 1 < QCoreApplication::arguments().size()) {
    const QDir dir(QCoreApplication::arguments().at(capture + 1));
    QDir().mkpath(dir.path());
    auto* window = qobject_cast<QQuickWindow*>(engine.rootObjects().value(0));
    auto shot = [window, dir](const QString& name) {
      if (window) window->grabWindow().save(dir.filePath(name + QStringLiteral(".png")));
    };
    auto* steps = new QList<std::pair<int, std::function<void()>>>{
        {1500, [shot, &controller] { shot(QStringLiteral("1-start")); controller.simulateDemo(QStringLiteral("moderate"), 1.0); }},
        {1500, [shot, &controller] { shot(QStringLiteral("2-setup")); controller.run(); }},
        {900, [shot] { shot(QStringLiteral("3-running")); }},
        {3200, [shot] { shot(QStringLiteral("4-results")); QCoreApplication::quit(); }},
    };
    auto* timer = new QTimer(&app);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, &app, [timer, steps] {
      if (steps->isEmpty()) return;
      auto step = steps->takeFirst();
      step.second();
      if (!steps->isEmpty()) timer->start(steps->first().first);
    });
    timer->start(steps->first().first);
  }
  return QGuiApplication::exec();
}
