
#include <QList>
#include <QObject>

class Graph : public QObject{
public:
	Graph ();
	void append(int timestamp, int value);


	QList<int> timestamps;
	QList<int> values;
private:
	;
};
