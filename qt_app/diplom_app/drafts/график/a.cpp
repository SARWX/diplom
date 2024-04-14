#include <QPushButton>
#include <QSizePolicy>
#include <QLineEdit>
#include <QLabel>
#include <QGraphicsItem>
// #include <QGraphicsPlotItem>

class GraphicsPlotNocksTube : public QGraphicsItem                          // Это вот мы создаем новый класс, который наследуется от QGraphicsItem
{
public:         // Публичные члены
    GraphicsPlotNocksTube(QGraphicsItem *parent): QGraphicsItem(parent){}   // Конструктор, который наследуется от QGraphicsItem
    void updateNocks(const QList<QGraphicsSimpleTextItem*>& nocks);         // Обновить отображаемый график (наверное)
    QRectF boundingRect()const {return m_boundRect;}                        // Этот метод возвращает прямоугольник границы (ну т.е. клетку на графике)
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *){}   // Разукрашивалка
    inline const QFont &font(){return m_NocksFont;}                         // Задавалка шрифтов
private:        // Приватные члены
    QList<QGraphicsSimpleTextItem*> m_nocks;        // m_nocks - список подписей 
    QFont m_NocksFont;                              // Шрифт подписей
    QPen m_nockPen;                                 // Стиль начертания текстаЫ
    QRectF m_boundRect;                             // Клетка
};

class Graphics2DPlotGrid: public QGraphicsItem      // Вот это непосредсвенно нащ график
{
public:
    Graphics2DPlotGrid(QGraphicsItem * parent);                 // Конструктор
    QRectF boundingRect() const;                                // Клетка
    const QRectF & rect() const;                                // ?
    void setRange(int axisNumber, double min, double max);      // Задать значения осей

    void setMainGrid(int axisNumber, double zero, double step);         // Разлиновка основная
    void setSecondaryGrid(int axisNumber, double zero, double step);    // Разлиновка дополнительная
    void setMainGridPen(const QPen & pen);                              // Установить начертание сетки основное
    void setSecondaryGridPen(const QPen &pen);                          // Установить начертание сетки дополнительное
    inline QPen mainGridPen(){return m_mainPen;}                        // Получить начертание сетки основное
    inline QPen secondaryGridPen(){return m_secondaryPen;}              // Получить начертание сетки дополнительное        

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);     // Тоже какая-то разукрашка
public:
    struct AxisGuideLines {
        AxisGuideLines(): showLines(true){}     // Конструктор структуры
        QVector<QLineF> lines;                  // Какой-то вектор линий
        bool showLines;                         // Отображать линии или нет
    };
    AxisGuideLines abscissMainLines;            // Абсциссы основные
    AxisGuideLines abscissSecondaryLines;       // Абсциссы дополнительные
    AxisGuideLines ordinateMainLines;           // Ординаты основные
    AxisGuideLines ordinateSecondaryLines;      // Ординаты дополнительные
private:

    void paintAxeGuidLines(const AxisGuideLines& axe, QPainter *painter, const QPen &linePen);

    QPen m_mainPen;
    QPen m_secondaryPen;

    QRectF m_rect;
};
void Graphics2DPlotGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    paintAxeGuidLines(abscissSecondaryLines, painter, m_secondaryPen);
        paintAxeGuidLines(abscissMainLines, painter, m_mainPen);
        paintAxeGuidLines(ordinateSecondaryLines, painter, m_secondaryPen);
        paintAxeGuidLines(ordinateMainLines, painter, m_mainPen);
    painter->setPen(m_mainPen);
    painter->drawRect(m_rect);
}

class GraphicsPlotItemPrivate
{
    Q_DECLARE_PUBLIC(GraphicsPlotItem)
    GraphicsPlotItem* q_ptr; 

    GraphicsPlotItemPrivate(GraphicsPlotItem* parent);
    void compose();
    void calculateAndSetTransForm();
    void autoSetRange();
    void autoSetGrid();
    void calculateOrdinateGrid();
    void calculateAbscissGrid();
    void setAxisRange(int axisNumber, double min, double max);

    Graphics2DPlotGrid * gridItem;
    QGraphicsSimpleTextItem * abscissText;
    QGraphicsSimpleTextItem * ordinateText;
    QGraphicsSimpleTextItem *titleText;
    QFont titleFont;
    QFont ordinaateFont;
    QFont abscissFont;

    QRectF rect;
    QRectF m_sceneDataRect;
    GraphicsPlotLegend *m_legend;
    GraphicsPlotNocksTube* ordinateMainNocks;
    GraphicsPlotNocksTube* ordinateSecondaryNocks;
    GraphicsPlotNocksTube* abscissSecondaryNocks;
    GraphicsPlotNocksTube* abscissMainNocks;

    struct Range{
        double min;
        double max;
    };
    struct AxisGuideLines {
        AxisGuideLines():baseValue(0.0), step(0.0){}
        double baseValue;
        double step;
    };
    AxisGuideLines abscissMainLines;
    AxisGuideLines abscissSecondaryLines;
    AxisGuideLines ordinateMainLines;
    AxisGuideLines ordinateSecondaryLines;

    Range abscissRange;
    Range ordinateRange;
    bool isAutoGrid;
    bool isAutoSecondaryGrid;

public:
    void range(int axisNumber, double *min, double *max);
};