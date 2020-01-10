// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "MyHudWidget.h"
#include "MySaveGame.h"
//#include "Runtime/Online/XMPP/Public/Xmpp.h"

#define LOCTEXT_NAMESPACE "HUD"

static UMyHudWidget *s_pUMyHudWidget;

UMyHudWidget::UMyHudWidget() : UUserWidget(FObjectInitializer())
{
	s_pUMyHudWidget = this;
}

UMyHudWidget* UMyHudWidget::GetMenu()
{
	return s_pUMyHudWidget;
}


/*void UMyHudWidget::ConnectToChatServerCompleted(const FXmppUserJid &oJid, bool bSuccess, const FString &sError)
{
	UE_LOG(LogGeneral, Error, TEXT("ConnectToChatServerCompleted"));
	FString UserName = TEXT("drones");
	bool bAval = FXmppModule::Get().IsAvailable();
	bool bIsEnabled = FXmppModule::Get().IsXmppEnabled();
	TSharedPtr<IXmppConnection> Connection = FXmppModule::Get().GetConnection(UserName);
	if (Connection.IsValid())
	{
		EXmppLoginStatus::Type eType = Connection->GetLoginStatus();
		if (eType == EXmppLoginStatus::LoggedIn)
		{
			int ttt = 0;
		}
		const FXmppUserJid &oJid = Connection->GetUserJid();
		int aaa = 0;
	}
	int eee = 0;
}
*/
void UMyHudWidget::Init()
{
	SetMana(GetMana());

/*	FXmppServer XmppServer;
	XmppServer.bUseSSL = false;
	XmppServer.ClientResource = TEXT("dron");

	XmppServer.ServerAddr = TEXT("138.201.246.149");
	XmppServer.Domain = TEXT("jabber.at");
	XmppServer.ServerPort = 5222;

	FString UserName = TEXT("drones");
	FString Password = TEXT("kkuukkuu78");

	TSharedPtr<IXmppConnection> Connection = FXmppModule::Get().GetConnection(UserName);

	if (Connection.IsValid() &&
		Connection->GetLoginStatus() == EXmppLoginStatus::LoggedIn)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Already logged in."));
		}
	}
	else
	{
		Connection = FXmppModule::Get().CreateConnection(UserName);
		Connection->SetServer(XmppServer);
		Connection->Login(UserName, Password);
		//Connection->OnLoginComplete().AddUFunction(this, "ConnectToChatServerCompleted");
		Connection->OnLoginComplete().AddUObject(this, &UMyHudWidget::ConnectToChatServerCompleted);
	}*/
}

int32 UMyHudWidget::GetMana()
{
	return UMySaveGame::pUMySaveGame->Mana;
}

void UMyHudWidget::SetMana(int32 iMana)
{
	UMySaveGame::pUMySaveGame->Mana = iMana;
	if (UMySaveGame::pUMySaveGame->Mana < 0)
	{
		UMySaveGame::pUMySaveGame->Mana = 0;
	}

	UMyHudWidget::GetMenu()->Mana = FText::Format(LOCTEXT("ManaFmt", "{0} x"), FText::AsNumber(UMySaveGame::pUMySaveGame->Mana));
	UMyHudWidget::GetMenu()->MaxMana = FText::Format(LOCTEXT("MaxManaFmt", "(Max:{0})"), UMySaveGame::pUMySaveGame->GetMaxMana());
}

void UMyHudWidget::AddMana(int32 iManaAdd)
{
	if (UMySaveGame::pUMySaveGame->Mana + iManaAdd > UMySaveGame::pUMySaveGame->GetMaxMana())
	{
		if (UMySaveGame::pUMySaveGame->Mana == UMySaveGame::pUMySaveGame->GetMaxMana())
		{
			return;
		}
		SetMana(UMySaveGame::pUMySaveGame->GetMaxMana());
		return;
	}
	SetMana(UMySaveGame::pUMySaveGame->Mana + iManaAdd);
}

void UMyHudWidget::Update()
{
	SetMana(GetMana());
}

#undef LOCTEXT_NAMESPACE