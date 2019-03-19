#ifndef TABLE_H
#define TABLE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QVector>


class Mark : public QGraphicsPolygonItem
{
public:
    Mark(const QPointF pos);

protected:
    QColor _Color;
};


class Cell : public QGraphicsRectItem
{
public:
    enum {Type = UserType + 1};

    Cell(const int row, const int column,
         const int cellSize, const QColor color);
    Cell();

    QPointF center() const;

    void check();
    bool isChecked();

    int row() const;
    int column() const;

    int type() const override;

private:
    bool _Checked;

    int _Row;
    int _Column;
};


class Horse : public QGraphicsPixmapItem
{
public:
    Horse(const int cellSize);

};



class Table : public QGraphicsScene
{
    Q_OBJECT
public:
    Table (QObject *parent);

    Cell* cellAt(const QPointF pos);

    void createField(const int row, const int column, const int cellSize);
    void clearField();

    bool isGameWon() const;
    bool isFirstStep() const;

    QColor black() const;
    QColor white() const;

    QSize size() const;

signals:
    void firstStep();
    void endGame(bool gameWon);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:    
    QVector<Cell *> horseSteps();

    bool _f_1Step;

    Horse *horse;

    QVector<QVector<Cell*>> field;
    int _checkedCells;

    const QColor _Black;
    const QColor _White;

    int _CellSize;

    int _Row;
    int _Column;
};

#endif // TABLE_H
