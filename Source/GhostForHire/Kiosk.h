// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GhostForHireCharacter.h"
#include "Kiosk.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API UKiosk : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UKiosk(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	// Loadout Slots
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//UButton* Slot1;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* Slot2;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* Slot3;

	//// Ability Keys
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* Button_Polt;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* Button_Polt4;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* Button_Tornado;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* Button_EctoSplat;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* Button_EctoHurl;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* Button_Screech;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* Button_ColdTouch;

	// Ability Bools
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool AB_Polt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool AB_Polt4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool AB_Tornado;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool AB_EctoSplat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool AB_EctoHurl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool AB_Screech;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool AB_ColdTouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool KB1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool KB2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool KB3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Clicked")
	bool KB4;


	// Functions
	UFUNCTION(BlueprintCallable)
	void AbilityToSlotI(int AbilityValueToInsert, int AbilityCost, bool AssignAB);
	UFUNCTION(BlueprintCallable)
	void AbilityToSlotII(int AbilityValueToInsert, int AbilityCost, bool AssignAB);
	UFUNCTION(BlueprintCallable)
	void AbilityToSlotIII(int AbilityValueToInsert, int AbilityCost, bool AssignAB);
	UFUNCTION(BlueprintCallable)
	void AbilityToSlotIV(int AbilityValueToInsert, int AbilityCost, bool AssignAB);
	UFUNCTION(BlueprintCallable)
	float DetermineCooldown(int key);
	UFUNCTION(BlueprintCallable)
	int DetermineIcon(int key);
	UFUNCTION(BlueprintCallable)
	void PutAbilityToSlot(int AbilityValueToInsert, int AbilityCost, bool AssignAB);

	// Player Character
	AGhostForHireCharacter* Player;

};
