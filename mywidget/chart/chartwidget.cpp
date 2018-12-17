#include "chartwidget.h"
#include "customslice.h"

ChartWidget::ChartWidget(QWidget *parent) : QWidget(parent)
{
    m_chart1 = new QChart();
    m_chartView1 = new QChartView(m_chart1, this);
    m_mainLayout = new QGridLayout();
    m_mainLayout->addWidget(m_chartView1, 0, 0, 3, 1);
    setLayout(m_mainLayout);


    m_series1 = new QBarSeries();
    addBarset();
    addBarset();
    addBarset();
    addBarset();

    m_chart1->addSeries(m_series1);
    m_chart1->setTitle("Legend detach example");
    m_chart1->createDefaultAxes();
//![1]
    m_chart1->legend()->setVisible(true);
    m_chart1->legend()->setAlignment(Qt::AlignBottom);
//![1]

    m_chartView1->setRenderHint(QPainter::Antialiasing);


    m_chart2 = new QChart;
    m_chart2->setTitle("Piechart customization");
    m_chart2->setAnimationOptions(QChart::AllAnimations);

    // create series
    m_series2 = new QPieSeries();
    *m_series2 << new CustomSlice("Slice 1", 10.0);
    *m_series2 << new CustomSlice("Slice 2", 20.0);
    *m_series2 << new CustomSlice("Slice 3", 30.0);
    *m_series2 << new CustomSlice("Slice 4", 40.0);
    *m_series2 << new CustomSlice("Slice 5", 50.0);
    m_series2->setLabelsVisible();
    m_chart2->addSeries(m_series2);

    m_chartView2 = new QChartView(m_chart2, this);
    m_mainLayout->addWidget(m_chartView2, 0, 1, 3, 1);
}

void ChartWidget::addBarset()
{
    QBarSet *barSet = new QBarSet(QString("set ") + QString::number(m_series1->count()));
    qreal delta = m_series1->count() * 0.1;
    *barSet << 1 + delta << 2 + delta << 3 + delta << 4 + delta;
    m_series1->append(barSet);
}
