#ifndef SERVO_H
#define SERVO_H

#include <QObject>

class servo : public QObject
{
    Q_OBJECT
public:
    explicit servo(QObject *parent = 0);

signals:

public slots:
};

#endif // SERVO_H