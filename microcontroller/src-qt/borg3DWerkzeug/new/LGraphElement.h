#include <QObject>


class LGraphElement : QObject {
	QOBJECT
	
public:
	LGraphElement(QObject* page);
	void addParent(LGraphElement* parent);
	void setChild(LGraphElement* child);
	
public slots:
	virtual void parentChanged();

private:
	QLinkedList<LGraphElement>* parents;
	LGraphElement* child;
	QObject* page;			// TODO muss noch auf die Konfigurationsklasse des 
							// der Operatorverwaltung umgeschrieben werden
	
	
};
