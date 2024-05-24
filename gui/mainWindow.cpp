#include "mainWindow.h"

mainWindow::mainWindow(QWidget* parent)
	: QWidget(parent), ui(new Ui::mainWindowClass)
{
	ui->setupUi(this);

	client.Connect("127.0.0.1", 8888);

	client.RecvRightsInfo();

	connect(ui->SignInToButton, &QPushButton::clicked, this, &mainWindow::SignInToButton_clicked);
	connect(ui->SignUpToButton, &QPushButton::clicked, this, &mainWindow::SignUpToButton_clicked);
}

void mainWindow::SignInToButton_clicked()
{
	std::cout << "Sign In\n";

	
	QString login = ui->LoginLineEdit->text();
	QString password = ui->PasswordLineEdit->text();

	if (!client.GetRights())
	{
		client.SendButtonInfo(ButtonType::SIGN_IN);
	}


	if (login == "admin" && password == "admin" && client.GetRights())
	{
		bool ok = false;

		QDialog dialog(this);
		dialog.setWindowTitle(tr("Change Password"));

		QVBoxLayout layout(&dialog);
		layout.setContentsMargins(20, 20, 20, 20);
		layout.setSpacing(10);

		QLabel label(tr("New Password:"));
		label.setStyleSheet("QLabel { background-color: transparent; color: white; font-size: 14px; }");
		layout.addWidget(&label);

		QLineEdit passwordLineEdit;
		passwordLineEdit.setEchoMode(QLineEdit::Password);
		passwordLineEdit.setStyleSheet("QLineEdit { background-color: #2c2c2c; color: white; border: 1px solid #4a4a4a; border-radius: 5px; padding: 5px; }");
		layout.addWidget(&passwordLineEdit);

		QHBoxLayout buttonsLayout;
		buttonsLayout.setSpacing(10);

		QPushButton okButton(tr("OK"));
		QPushButton cancelButton(tr("Cancel"));

		okButton.setStyleSheet("QPushButton { background-color: green; color: white; border-radius: 5px; padding: 10px; }"
			"QPushButton:hover { background-color: #00a000; }");
		cancelButton.setStyleSheet("QPushButton { background-color: red; color: white; border-radius: 5px; padding: 10px; }"
			"QPushButton:hover { background-color: #d00000; }");

		buttonsLayout.addWidget(&okButton);
		buttonsLayout.addWidget(&cancelButton);
		layout.addLayout(&buttonsLayout);

		QObject::connect(&okButton, &QPushButton::clicked, [&]() {
			if (passwordLineEdit.text().isEmpty())
			{
				QMessageBox::warning(&dialog, tr("Change Password"), tr("Password cannot be empty!"));
			}
			else
			{
				ok = true;
				dialog.accept();
			}
		});

		QObject::connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

		dialog.setStyleSheet("QDialog { background-color: #444444; }");
		dialog.exec();

		if (ok)
		{
			QString newPassword = passwordLineEdit.text();
			std::cout << "New password: " << newPassword.toStdString() << '\n';
			bool check_flag = true;
			client.SendCheckFlag(check_flag);
			client.SendAdminPassword(newPassword.toStdString());
			return;
		}
	}

	bool check_flag = false;

	if (login.isEmpty() || password.isEmpty())
	{
		QMessageBox::critical(this, "Message error", "Login and Password could not be empty!");
		client.SendCheckFlag(check_flag);
		return;
	}
	else if (login.size() > 255 || password.size() > 255)
	{
		QMessageBox::critical(this, "Message error", "Login and Password could not be greater than 255 symbols!");
		client.SendCheckFlag(check_flag);
		return;
	}
	else
	{
		std::cout << "Sending user info for sign in\n";
		check_flag = true;
		client.SendCheckFlag(check_flag);
		if (client.SendUserInfoSignIn(login.toStdString(), password.toStdString()))
		{
			QMessageBox::critical(this, "Message error", "Sing in successfull");
		}
	}
}

void mainWindow::SignUpToButton_clicked()
{
	QString login = ui->LoginLineEdit->text();
	QString password = ui->PasswordLineEdit->text();

	client.SendButtonInfo(ButtonType::SIGN_UP);

	bool check_flag = false;

	if (login.isEmpty() || password.isEmpty() || login == "admin")
	{
		QMessageBox::critical(this, "Message error", "Login and Password could not be empty and could not be admin!");
		client.SendCheckFlag(check_flag);
		return;
	}
	else if (login.size() > 255 || password.size() > 255)
	{
		QMessageBox::critical(this, "Message error", "Login and Password could not be greater than 255 symbols!");
		client.SendCheckFlag(check_flag);
		return;
	}
	else
	{
		check_flag = true;
		client.SendCheckFlag(check_flag);
		if (client.SendUserInfoSignUp(login.toStdString(), password.toStdString()))
		{
			QMessageBox::about(this, "Message", "Sing up is successfull");
		}
		else
		{
			QMessageBox::critical(this, "Error", "Login and Password!");	
		}
	}
}

mainWindow::~mainWindow()
{
	delete ui;
}