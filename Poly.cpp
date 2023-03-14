#include "Poly.h"

Poly::Poly()
{
	head = new PolyNode(-1, 0, NULL);
}

Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{
	head = new PolyNode(-1, 0, NULL);
	for (int i = deg.size() - 1; i >= 0; i--) {
		head->next = new PolyNode(deg[i], coeff[i], head->next);
	}
}

Poly::~Poly()
{
	while (head->next != NULL) {
		PolyNode* temp = head->next;
		head->next = temp->next;
		delete temp;
	}
}

void Poly::addMono(int i, double c)
{
	//T(n) = O(n)
	//S(n) = Theta(n)
	if (c != 0) {
		PolyNode* read = head;
		while (read->next != NULL && read->next->deg > i) {
			read = read->next;
		}
		if (read->next == NULL) {
			read->next = new PolyNode(i, c, NULL);
		}
		else if (read->next->deg == i) {
			read->next->coeff += c;
			if (read->next->coeff == 0) {
				PolyNode* temp = read->next;
				read->next = temp->next;
				delete temp;
			}
		}
		else {
			read->next = new PolyNode(i, c, read->next);
		}
	}
}

void Poly::addPoly(const Poly& p)
{
	//T(n) = O(n^2)
	//S(n) = O(n^2)
	for (PolyNode* read = p.head; read->next != NULL; read = read->next) {
		addMono(read->next->deg, read->next->coeff);
	}
}

void Poly::multiplyMono(int i, double c)
{
	//T(n) = Theta(n)
	//S(n) = Theta(n)
	if (c == 0) {
		this->~Poly();
	}
	else {
		for (PolyNode* read = head; read->next != NULL; read = read->next) {
			read->next->deg += i;
			read->next->coeff *= c;
		}
	}
}

void Poly::multiplyPoly(const Poly& p)
{
	//T(n) = Theta(n^2*m) m is the length of p
	//S(n) = Theta(m)
	Poly* result = new Poly();

	if (this->isZero() || p.isZero()) {
		this->~Poly();
		return;
	}

	PolyNode* read = p.head->next;

	while (read) {
		if (read->coeff == 0) {
			read = read->next;
			continue;
		}

		PolyNode* read2 = head->next;

		while (read2) {
			if (read2->coeff == 0) {
				read2 = read2->next;
				continue;
			}

			result->addMono(read->deg + read2->deg, read->coeff * read2->coeff);
			read2 = read2->next;
		}

		read = read->next;
	}

	result->duplicate(*this);
	delete result;
}

void Poly::duplicate(Poly& outputPoly)
{
	//T(n) = Theta(n)
	//S(n) = Theta(n)
	PolyNode* temp = outputPoly.head;
	for (PolyNode* read = head; read->next != NULL; read = read->next) {
		temp->next = new PolyNode(read->next->deg, read->next->coeff, NULL);
		temp = temp->next;
	}
}

int Poly::getDegree()
{
	//T(n) = Theta(1)
	//S(n) = Theta(n)
	if (head->next != NULL) {
		return head->next->deg;
	}
	return -1;
}

int Poly::getTermsNo()
{
	//T(n) = Theta(n)
	//S(n) = Theta(n)
	int size = 0;
	for (PolyNode* read = head; read->next != NULL; read = read->next) {
		size++;
	}
	return size;
}

double Poly::evaluate(double x)
{
	double result = 0;
	for (PolyNode* read = head; read->next != NULL; read = read->next) {
		result += (read->next->coeff) * (pow(x, read->next->deg));
	}
	return result;
}

std::string Poly::toString()
{
	std::string s = "degree=";
	if (head->next == NULL) {
		return "degree=-1";
	}
	s += std::to_string(head->next->deg) + "; ";
	PolyNode* read = head;
	while (read->next != NULL) {
		s += "a(" + std::to_string(read->next->deg) + ")=" + std::to_string(read->next->coeff) + "; ";
	}
	return s;
}
