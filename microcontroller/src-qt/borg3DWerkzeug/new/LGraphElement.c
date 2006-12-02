#inlude "LGraphElement.h"

LGraphElement::LGraphElement(QObject* page) {
	this->page = page;
	parents = new QLinkedList<LGraphElement>();
	child = NULL;
}

void LGraphElement::addParent(LGraphElement* parent) {
	parents->
}

void LGraphElement::setChild(LGraphElement* child) {

}
