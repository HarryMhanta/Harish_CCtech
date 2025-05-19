#pragma once

#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>

class PolygonExtrusionWidgetUI {
public:
    QComboBox* booleanComboBox;
    QCheckBox* subtractionModeCheckBox;
    QPushButton* applyBooleanButton;
    QVBoxLayout* layout;

    PolygonExtrusionWidgetUI(QWidget* parent);
};