#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

struct Payment
{
	string sender;
	string receiver;
	double money;
	string currency;
	SYSTEMTIME date;
	string comment;
};

// Абстрактный класс, представляющий обработчик платежей
class PaymentHandler {
protected:
	PaymentHandler* nextHandler = nullptr;
public:
	virtual void handlePayment(Payment& details) = 0;
	virtual void setNextHandler(PaymentHandler* nextHandler) = 0;
};

// Обработчик платежей, который может обработать платежи
class BasePaymentHandler : public PaymentHandler {
private:
	bool checkPaymentRequisites() {
		cout << "проверяем правильность указанных данных по счёту отправителя и получателя...\n";
		Sleep(1000);
		return true;
	}

	bool checkEnoughMoney() {
		cout << "проверяем наличие достаточного количества денежных средств на счету отправителя (c учётом комиссии, разумеется)...\n";
		Sleep(1000);
		return true;
	}

public:
	void handlePayment(Payment& details) override {
		cout << "инициирована транзакция по переводу денежных средств\nсо счёта ";
		cout << details.sender << " на счёт " << details.receiver << "\n";
		Sleep(1000);

		checkPaymentRequisites();
		checkEnoughMoney();

		if (nextHandler != nullptr) {
			nextHandler->handlePayment(details);
		}
		else {
			cout << "базовый обработчик платежа выполнен.\n";
			Sleep(1000);
		}
	}

	void setNextHandler(PaymentHandler* next) override { // !!!
		nextHandler = next;
	}
};

// Обработчик платежей, который обрабатывает платежи от 5000 гривен
class BigMoneyPaymentHandler : public PaymentHandler {
private:
	bool finmonitoring() {
		return rand() % 2; // тут уж как повезёт
	}

public:
	void handlePayment(Payment& details) override {
		if (details.money > 5000.0f) {
			cout << "запускаем дополнительную проверку счетов по процедуре фин.мониторинга...\n";
			finmonitoring();
			Sleep(1000);
		}
		
		if (nextHandler != nullptr) {
			nextHandler->handlePayment(details);
		}
		else {
			cout << "обработчик финмониторинга суммы от 5000грн выполнен.\n";
			Sleep(1000);
		}
	}

	void setNextHandler(PaymentHandler* next) override {
		nextHandler = next;
	}
};

// Обработчик платежей, который проверяет дату прошлой транзакции
class SuspiciousActivityPaymentHandler : public PaymentHandler {
private:
	int daysFromLastPayment()
	{
		return 5; // например, прошлый платёж был совершён 5 дней назад
	}
public:
	void handlePayment(Payment& details) override {
		cout << "проверяем дату прошлого платежа...\n";
		Sleep(1000);

		if (daysFromLastPayment() < 365)
		{
			cout << "всё ОК...\n";
			Sleep(1000);
		}
		else {
			cout << "обнаружена подозрительная активность!\n";
		}

		if (nextHandler != nullptr) {
			nextHandler->handlePayment(details);
		}
		else {
			cout << "обработчик подозрительной активности выполнен.\n";
		}
	}

	void setNextHandler(PaymentHandler* next) override {
		nextHandler = next;
	}
};

// Обработчик платежей, который проверяет наличие комментария к переводу
class CommentPaymentHandler : public PaymentHandler {
public:
	void handlePayment(Payment& details) override {
		cout << "проверяем наличие комментария к платежу...\n";
		Sleep(1000);

		if (details.comment == "")
		{
			cout << "комментария нет!!\n";
			Sleep(1000);
		}
		else {
			cout << "комментарий есть!\n";
			Sleep(1000);
		}

		if (nextHandler != nullptr) {
			nextHandler->handlePayment(details);
		}
		else {
			cout << "транзакция прошла успешно, хорошего дня!\n";
		}
	}

	void setNextHandler(PaymentHandler* next) override {
		nextHandler = next;
	}
};

int main() {
	setlocale(0, "");
	// Создаем объекты обработчиков платежей
	PaymentHandler* handler = new BasePaymentHandler();
	PaymentHandler* isBigSum = new BigMoneyPaymentHandler();
	PaymentHandler* hasComment = new CommentPaymentHandler();

	handler->setNextHandler(isBigSum);
	isBigSum->setNextHandler(hasComment);

	Payment payment;
	payment.sender = "5375411419283745";
	payment.receiver = "5375411428374650";
	payment.currency = "UAH";
	SYSTEMTIME time;
	GetSystemTime(&time);
	payment.date = time;
	payment.money = 1000;
	payment.comment = "оплата аренды помещения на основании договора №17 от 12.01.2023";
	
	// Обрабатываем платеж
	handler->handlePayment(payment);

	// Освобождаем память, занятую объектами обработчиков платежей
	delete handler;
	delete isBigSum;
	delete hasComment;
}

/*
Данный код на языке C++ реализует шаблон проектирования
"Цепочка ответственности" (Chain of Responsibility).

Основная идея заключается в том, что у нас есть несколько
обработчиков запроса (PaymentHandler), которые могут
обрабатывать запрос, и каждый обработчик знает только
о своих возможностях. Если один обработчик не может
выполнить запрос, то он перенаправляет его следующему
обработчику.

Структура Payment определяет поля, описывающие информацию
о платеже, такие как отправитель, получатель, сумма и дата.

Абстрактный класс PaymentHandler определяет интерфейс,
который должны реализовывать все обработчики запроса.
Классы BasePaymentHandler, BigMoneyPaymentHandler,
SuspiciousActivityPaymentHandler и CommentPaymentHandler -
это классы-обработчики, которые реализуют данный интерфейс.

Метод handlePayment() каждого обработчика проверяет,
соответствует ли платеж условиям, которые он может
обработать, и если да, то он выполняет необходимые
действия и передает управление следующему обработчику.

В main() создаются объекты обработчиков платежей и
устанавливается порядок их вызова с помощью метода
setNextHandler().

Обработчик BasePaymentHandler проверяет правильность
указанных данных по счету отправителя и получателя и
наличие достаточного количества денежных средств на
счету отправителя.

Обработчик BigMoneyPaymentHandler проверяет, что сумма
платежа больше 5000 гривен и запускает дополнительную
проверку счетов по процедуре финансового мониторинга.

Обработчик SuspiciousActivityPaymentHandler проверяет,
что прошло не более 365 дней с момента предыдущей
транзакции.

Обработчик CommentPaymentHandler проверяет наличие
комментария к платежу.
*/

// https://refactoring.guru/ru/design-patterns/chain-of-responsibility

/*
Цепочка обязанностей — это поведенческий паттерн
проектирования, который позволяет передавать запросы
последовательно по цепочке обработчиков. Каждый
последующий обработчик решает, может ли он обработать
запрос сам и стоит ли передавать запрос дальше по цепи.
*/