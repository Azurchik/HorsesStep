#include "table.h"

#include <QRandomGenerator>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QtAlgorithms>

#include <QDebug>

Table::Table(QObject *parent)
    : QGraphicsScene (parent)
    , _Black (97, 20, 39)
    , _White (221, 220, 197)
{
}


Cell* Table::cellAt(const QPointF pos)
{
    for (QVector<Cell*> &row: field) {
        for (Cell *cell: row) {
            if (cell->pos() == pos) {
                return cell;
            }
        }
    }
    return new Cell();
}


void Table::createField(const int row, const int column, const int cellSize)
{
    clearField();
    delete horse;
    clear();

    _f_1Step      = true;
    _checkedCells = 0;
    _CellSize     = cellSize;
    _Row          = row;
    _Column       = column;

    setSceneRect(0, 0, _Column * _CellSize, _Row * _CellSize);

    int y = 0;
    for (int i = 0; i < _Row; ++i) {
        field.push_back({});
        int x = 0;
        for (int j = 0; j < _Column; ++j) {
            QColor color;
            if (i % 2 != 0) {
                if ( j % 2 != 0) {
                    color = _White;
                } else {
                    color = _Black;
                }
            } else {
                if ( j % 2 != 0) {
                    color = _Black;
                } else {
                    color = _White;
                }
            }
            Cell *cell = new Cell(i, j, _CellSize,  color);
            field[i].push_back(cell);
            addItem(cell);
            cell->setPos(x, y);

            x += _CellSize;
        }
        y += _CellSize;
    }

    int r = QRandomGenerator::global()->bounded(_Row);
    int c = QRandomGenerator::global()->bounded(_Column);

    horse = new Horse(_CellSize);
    horse->setPos(field[r][c]->pos());
    addItem(horse);
}

void Table::clearField()
{
    for (int y = 0; y < _Row; ++y) {
        for (int x = 0; x < _Column; ++x) {
            delete field[y][x];
        }
    }
    field.clear();
}


bool Table::isGameWon() const
{
    return _checkedCells == _Row * _Column;
}

bool Table::isFirstStep() const
{
    return _f_1Step;
}


QColor Table::black() const
{
    return _Black;
}

QColor Table::white() const
{
    return _White;
}


QSize Table::size() const
{
    return QSize(_CellSize * _Column, _CellSize * _Row);
}


void Table::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform(Qt::Initialization::Uninitialized));

    if (item->type() == Cell::Type) {
        Cell *cell = cellAt(item->pos());

        bool detected = false;
        for (Cell *cll: horseSteps()) {
            if (cell->pos() == cll->pos()) {
                detected = true;
                break;
            }
        }

        if (detected) {
            if (!_f_1Step) {
                addItem(new Mark(cellAt(horse->pos())->center()));
            } else {
                emit firstStep();
                _f_1Step = false;
            }
            _checkedCells++;
            cell->check();
            horse->setPos(cell->pos());
        }
    }

    if (horseSteps().count() == 0) {
        emit endGame(isGameWon());
    }

    Q_UNUSED(event);
}

QVector<Cell *> Table::horseSteps()
{
    Cell *horse_ = cellAt(horse->pos());
    int row    = horse_->row();
    int column = horse_->column();

    QVector<Cell *> result;
    if (row + 2 < _Row) {
        if (column + 1 < _Column)
            if (!field[row + 2][column + 1]->isChecked())
                result.push_back(field[row + 2][column + 1]); // BottomRight
        if (column - 1 >= 0)
            if (!field[row + 2][column - 1]->isChecked())
                result.push_back(field[row + 2][column - 1]); // BottomLeft
    }
    if (row - 2 >= 0) {
        if (column + 1 < _Column)
            if (!field[row - 2][column + 1]->isChecked())
                result.push_back(field[row - 2][column + 1]); // TopRight
        if (column - 1 >= 0)
            if (!field[row - 2][column - 1]->isChecked())
                result.push_back(field[row - 2][column - 1]); // TopLeft
    }

    if (column + 2 < _Column) {
        if (row + 1 < _Row)
            if (!field[row + 1][column + 2]->isChecked())
                result.push_back(field[row + 1][column + 2]); // RightBottom
        if (row - 1 >= 0)
            if (!field[row - 1][column + 2]->isChecked())
                result.push_back(field[row - 1][column + 2]); // RightTop
    }
    if (column - 2 >= 0) {
        if (row + 1 < _Row)
            if (!field[row + 1][column - 2]->isChecked())
                result.push_back(field[row + 1][column - 2]); // LeftBotttom
        if (row - 1 >= 0)
            if (!field[row - 1][column - 2]->isChecked())
                result.push_back(field[row - 1][column - 2]); // LeftTop
    }
    return result;
}



Cell::Cell(const int row, const int column, const int cellSize, const QColor color)
    : QGraphicsRectItem(nullptr)
    , _Checked (false)
    , _Row     (row)
    , _Column  (column)
{
    setBrush(color);
    setRect(0, 0, cellSize, cellSize);
}

Cell::Cell()
    : _Row    (-1)
    , _Column (-1)
{}

QPointF Cell::center() const
{
    double factor = rect().width() / 2;
    return QPointF(pos().x() + factor, pos().y() + factor);
}

void Cell::check()
{
    _Checked = true;
}

bool Cell::isChecked()
{
    return _Checked;
}

int Cell::row() const
{
    return _Row;
}

int Cell::column() const
{
    return _Column;
}

int Cell::type() const
{
    return Type;
}



Horse::Horse(const int cellSize)
    : QGraphicsPixmapItem(nullptr)
{
    setPixmap(QPixmap(":/image/horse3.png").scaled(cellSize, cellSize));
}



Mark::Mark(const QPointF pos)
    : QGraphicsPolygonItem(nullptr)
    , _Color (29, 35, 38)
{
    setBrush(_Color);
    QPolygon polygon;
    polygon << QPoint(0, 10)   << QPoint(10, 0)
            << QPoint (0, -10) << QPoint(-10, 0);
    setPolygon(polygon);
    setPos(pos);
}
