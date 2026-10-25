#include "blot_image_provider.hpp"

#include "app_controller.hpp"

BlotImageProvider::BlotImageProvider(const AppController* controller)
    : QQuickImageProvider(QQuickImageProvider::Image), controller_(controller) {}

QImage BlotImageProvider::requestImage(const QString& id, QSize* size, const QSize& requested_size) {
  QImage image = controller_->blotImage(id.section(QLatin1Char('/'), 0, 0).toInt());
  if (size) *size = image.size();
  if (requested_size.isValid() && !image.isNull())
    image = image.scaled(requested_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  return image;
}
