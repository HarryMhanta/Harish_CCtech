#include "PolygonExtrusionWidgetUI.h"

PolygonExtrusionWidgetUI::PolygonExtrusionWidgetUI(QWidget* parent) {
    booleanComboBox = new QComboBox(parent);
    booleanComboBox->addItem("Union");
    booleanComboBox->addItem("Subtraction");
    booleanComboBox->addItem("Intersection");
    booleanComboBox->setFixedSize(100, 25);

    subtractionModeCheckBox = new QCheckBox("A - B", parent);
    subtractionModeCheckBox->setChecked(true);
    subtractionModeCheckBox->setFixedSize(100, 20);

    applyBooleanButton = new QPushButton("Apply", parent);
    applyBooleanButton->setFixedSize(100, 25);

    layout = new QVBoxLayout();
    layout->addWidget(booleanComboBox);
    layout->addWidget(subtractionModeCheckBox);
    layout->addWidget(applyBooleanButton);
    layout->setAlignment(Qt::AlignTop | Qt::AlignRight);
    layout->setContentsMargins(10, 10, 10, 10);
}