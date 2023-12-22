#ifndef HORIZONTALMOVEABLERECTITEM_H
#define HORIZONTALMOVEABLERECTITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class HorizontalMovableRectItem : public QGraphicsRectItem
{
public:
    HorizontalMovableRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr)
        : QGraphicsRectItem(x, y, width, height, parent)
    {
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override
    {
        if (change == ItemPositionChange && scene()) {
            // Ensure only horizontal movement
            QPointF newPos = value.toPointF();
            return QPointF(newPos.x(), pos().y());
        }

        return QGraphicsRectItem::itemChange(change, value);
    }
};


#endif // HORIZONTALMOVEABLERECTITEM_H
