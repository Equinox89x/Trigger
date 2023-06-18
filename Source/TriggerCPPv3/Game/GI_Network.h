#pragma once

#include "CoreMinimal.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "GI_Network.generated.h"

/**
*
*/
UCLASS()
class TRIGGERCPPV3_API UGI_Network : public UGameInstance
{
	GENERATED_BODY()

public:

	UGI_Network();

	#pragma region Spawn/Game/UI Info
	int Seconds{ 15 };

	int SelectedSpawnIndex{ 0 };

	float Distance{ 600 };

	int MaxBullets{ 0 };

	int PlayerAmount{ 2 };
	#pragma endregion

	void StartMatch(const EName& sessionName);
	void EndMatch(const EName& sessionName);

protected:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	IOnlineSessionPtr SessionInterface;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	//virtual void OnCreateSessionFailed(FName SessionName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	virtual void OnDestroySessionComplete(FName SessionName, bool Succeeded);
	virtual void OnEndSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnSessionFailed(const FUniqueNetId& id, ESessionFailure::Type fail);


	UFUNCTION(BlueprintCallable)
		void CreateServer(int playerAmount, FString mapName, FString gameMode, bool isLan);
	UFUNCTION(BlueprintCallable)
		void JoinServer();
	UFUNCTION(BlueprintCallable)
		void FindServers(FString mapName, FString gameMode, bool isLan);

	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	UFUNCTION(BlueprintCallable)
		FString GetFirstServerName() const { return FirstSession.GetSessionIdStr(); };
	//UFUNCTION(BlueprintCallable)
	//	FOnlineSessionSearchResult GetSessionResult() { return FirstSession; };

private:
	APlayerController* CurrentPlayer{};

	FOnlineSessionSearchResult FirstSession {};
};
