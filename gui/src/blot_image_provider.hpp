#pragma once

#include <QQuickImageProvider>

class AppController;

// Serves blot display images to QML as image://blot/<blot index>/<revision>.
// The revision part only defeats QML's image cache after reloading.
class BlotImageProvider : public QQuickImageProvider {
 public:
  explicit BlotImageProvider(const AppController* controller);
  QImage requestImage(const QString& id, QSize* size, const QSize& requested_size) override;

 private:
  const AppController* controller_;
};
