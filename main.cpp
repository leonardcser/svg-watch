#include <QApplication>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QGestureEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPainter>
#include <QPinchGesture>
#include <QSvgRenderer>
#include <QWheelEvent>
#include <QWidget>

class SvgWidget : public QWidget {
  public:
    SvgWidget(const QString &svgFilePath, QWidget *parent = nullptr)
        : QWidget(parent), renderer(nullptr), scaleFactor(1.0), offset(0, 0) {
        if (!QFileInfo::exists(svgFilePath)) {
            QMessageBox::critical(this, "Error", "SVG file not found!");
            return;
        }
        // Initializing the renderer with the SVG file
        loadSvg(svgFilePath);

        this->setStyleSheet("background-color: white;");
        setAttribute(Qt::WA_AcceptTouchEvents, true);
        grabGesture(Qt::PinchGesture);

        // Setting up the file system watcher
        fileWatcher.addPath(svgFilePath);
        connect(&fileWatcher, &QFileSystemWatcher::fileChanged, this,
                &SvgWidget::onFileChanged);
    }

  protected:
    void paintEvent(QPaintEvent *event) override {
        if (renderer) {
            QPainter painter(this);

            // Apply scaling and translation
            painter.translate(offset);
            painter.scale(scaleFactor, scaleFactor);

            // Render the SVG at its default size, transformed by scaling and
            // offset
            QRectF targetRect(0, 0, renderer->defaultSize().width(),
                              renderer->defaultSize().height());
            renderer->render(&painter, targetRect);
        }
    }

    void wheelEvent(QWheelEvent *event) override {
        // Scroll/pan normally
        offset +=
            QPoint(event->angleDelta().x() / 4, event->angleDelta().y() / 4);
        update();
    }

    bool event(QEvent *event) override {
        if (event->type() == QEvent::Gesture) {
            return handleGesture(static_cast<QGestureEvent *>(event));
        }
        return QWidget::event(event);
    }

    bool handleGesture(QGestureEvent *event) {
        if (QGesture *pinch = event->gesture(Qt::PinchGesture)) {
            QPinchGesture *gesture = static_cast<QPinchGesture *>(pinch);
            if (gesture->state() == Qt::GestureUpdated) {
                // Calculate the scaling factor
                qreal scale = gesture->scaleFactor();
                if (scaleFactor * scale >= MIN_SCALE_FACTOR) {
                    scaleFactor *= scale;
                    // Calculate the new offset to keep the center in place
                    QPointF center = rect().center();
                    QPointF deltaCenter = center - offset;
                    offset = QPoint(center.x() - deltaCenter.x() * scale,
                                    center.y() - deltaCenter.y() * scale);

                    update();
                }
            }
        }
        return true;
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_F) {
            // Calculate the scaling factor to fit the SVG into the window
            QRect widgetRect = this->rect();
            QSize svgSize = renderer->defaultSize();

            qreal scaleX =
                widgetRect.width() / static_cast<qreal>(svgSize.width());
            qreal scaleY =
                widgetRect.height() / static_cast<qreal>(svgSize.height());

            // Scale the SVG to fit within the window, maintaining aspect ratio
            scaleFactor = qMin(scaleX, scaleY);

            // Recalculate offset to keep SVG centered
            offset = QPoint(
                (widgetRect.width() - svgSize.width() * scaleFactor) / 2,
                (widgetRect.height() - svgSize.height() * scaleFactor) / 2);

            update();
        }
        QWidget::keyPressEvent(event);
    }

  private:
    const qreal MIN_SCALE_FACTOR = 0.1;
    QSvgRenderer *renderer;
    qreal scaleFactor; // Zoom factor
    QPoint offset;     // Offset for panning
    QFileSystemWatcher fileWatcher;

    void loadSvg(const QString &svgFilePath) {
        if (renderer) {
            // Delete the old renderer if it exists
            delete renderer;
        }
        renderer = new QSvgRenderer(svgFilePath, this);
    }

    void onFileChanged(const QString &filePath) {
        // Reload the SVG when the file is modified
        loadSvg(filePath);
        update();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(QString("SVG Watch"));

    if (argc != 2) {
        qCritical() << "Usage: " << argv[0] << "<SVG file path>";
        return -1;
    }

    QString svgFilePath = argv[1];
    SvgWidget window(svgFilePath);
    window.resize(800, 600);
    window.show();
    window.setWindowTitle(app.applicationName());

    return app.exec();
}