#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QtCharts>
using namespace QtCharts;

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    QChart *m_chart1;
    QBarSeries *m_series1;

    QChartView *m_chartView1;
    QGridLayout *m_mainLayout;

    QChart *m_chart2;
    QPieSeries *m_series2;

    QChartView *m_chartView2;

    void addBarset();
};

#endif // CHARTWIDGET_H
