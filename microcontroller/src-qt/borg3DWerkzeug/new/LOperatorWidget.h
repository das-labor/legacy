#include <QWidget>
#include <QString>

/**
 * Main class of all grafical operators.
 */
class LOperatorWidget : QWidget {
	QOBJECT
	
public:
	LOperatorBase(QString name);
	
	QString getName();
	virtual QWidget getConfigWidget();
	TOut *getData();
	
public slots:
	void parentChanged();	
	
signals:
	void finished();
	
private:
	QString name;
};