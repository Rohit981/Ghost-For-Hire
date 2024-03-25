// Fill out your copyright notice in the Description page of Project Settings.


#include "Kiosk.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/Engine.h"

UKiosk::UKiosk(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UKiosk::NativeConstruct()
{
    Super::NativeConstruct();
    Player = Cast<AGhostForHireCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    //MyTextBlock->SetText(FText::FromString("Default Text"));

    //int test = 5;
    //if (Slot1)
    //{
    //    //MyTextBlock->SetText(FText::FromString("Default Text"));
    //    //MyTextBlock->SetText(FText::AsNumber(test));
    //    //Slot1->SetVisibility(ESlateVisibility::Visible);
    //    Slot1->OnClicked.AddDynamic(this, &UKiosk::AbilityToSlot);
    //    //NameTextBlock->SetText(FText::FromString("Test Text"));
    //}
}




void UKiosk::AbilityToSlotI(int AbilityValueToInsert, int AbilityCost, bool AssignAB)
{
    if (KB1)
    {

        if (Player != nullptr) 
        {
            if (Player->K_1 != AbilityValueToInsert && Player->K_2 != AbilityValueToInsert && Player->K_3 != AbilityValueToInsert && Player->K_4 != AbilityValueToInsert)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Here"));
                if (AssignAB)
                {
                    KB1 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    Player->K_1 = AbilityValueToInsert;
                    AssignAB = true;
                }
                else
                {
                    Player->GPM_Funds -= AbilityCost;
                    KB1 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("value: %d"), AbilityKeyToChange));
                    Player->K_1 = AbilityValueToInsert;
                    AssignAB = true;
                }
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("SLOT TAKEn"));
            }
        }

    }
}

void UKiosk::AbilityToSlotII(int AbilityValueToInsert, int AbilityCost, bool AssignAB)
{
    if (KB2)
    {

        if (Player != nullptr)
        {
            if (Player->K_1 != AbilityValueToInsert && Player->K_2 != AbilityValueToInsert && Player->K_3 != AbilityValueToInsert && Player->K_4 != AbilityValueToInsert)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Here"));
                if (AssignAB)
                {
                    KB2 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    Player->K_2 = AbilityValueToInsert;
                    AssignAB = true;
                }
                else
                {
                    Player->GPM_Funds -= AbilityCost;
                    KB2 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("value: %d"), AbilityKeyToChange));
                    Player->K_2 = AbilityValueToInsert;
                    AssignAB = true;
                }
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("SLOT TAKEn"));
            }
        }

    }
}

void UKiosk::AbilityToSlotIII(int AbilityValueToInsert, int AbilityCost, bool AssignAB)
{
    if (KB3)
    {

        if (Player != nullptr)
        {
            if (Player->K_1 != AbilityValueToInsert && Player->K_2 != AbilityValueToInsert && Player->K_3 != AbilityValueToInsert && Player->K_4 != AbilityValueToInsert)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Here"));
                if (AssignAB)
                {
                    KB3 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    Player->K_3 = AbilityValueToInsert;
                    AssignAB = true;
                }
                else
                {
                    Player->GPM_Funds -= AbilityCost;
                    KB3 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("value: %d"), AbilityKeyToChange));
                    Player->K_3 = AbilityValueToInsert;
                    AssignAB = true;
                }
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("SLOT TAKEn"));
            }
        }

    }
}

void UKiosk::AbilityToSlotIV(int AbilityValueToInsert, int AbilityCost, bool AssignAB)
{
    if (KB4)
    {

        if (Player != nullptr)
        {
            if (Player->K_1 != AbilityValueToInsert && Player->K_2 != AbilityValueToInsert && Player->K_3 != AbilityValueToInsert && Player->K_4 != AbilityValueToInsert)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Here"));
                if (AssignAB)
                {
                    KB4 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    Player->K_4 = AbilityValueToInsert;
                    AssignAB = true;
                }
                else
                {
                    Player->GPM_Funds -= AbilityCost;
                    KB4 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("value: %d"), AbilityKeyToChange));
                    Player->K_4 = AbilityValueToInsert;
                    AssignAB = true;
                }
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("SLOT TAKEn"));
            }
        }

    }
}

float UKiosk::DetermineCooldown(int key)
{
    int ab_max = 11;

    float cd_P3 = 15.0f;
    float cd_P4 = 30.0f;
    float cd_Tornado = 300.0f;
    float cd_EctoHurl = 15.0f;
    float cd_EctoSplat = 15.0f;
    float cd_Screech = 30.0f;
    float cd_Tamper = 15.0f;
    float cd_Rattle = 5.0f;
    float cd_ColdTouch = 20.0f;
    //float cd_

    bool tempBool;
    float ab_cd;
    float max_cd;

    //for(int i = 1; i <= ab_max; i++)
    //{
    //    if (Player->K_1 == i)
    //    {
    //        //tempBool = ReturnAbilityBool();
    //        if (Player->K_1 == 1)
    //        {
    //            tempBool = Player->IsP3Cooldown;
    //            ab_cd = Player->P3_Cooldown;
    //            max_cd = cd_P3;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_1 == 2)
    //        {
    //            tempBool = Player->IsScreechCooldown;
    //            ab_cd = Player->Screech_Cooldown;
    //            max_cd = cd_Screech;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_1 == 3)
    //        {
    //            tempBool = Player->IsP4Cooldown;
    //            ab_cd = Player->P4_Cooldown;
    //            max_cd = cd_P4;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_1 == 4)
    //        {
    //            tempBool = Player->IsEctoHurlCooldown;
    //            ab_cd = Player->EctoHurl_Cooldown;
    //            max_cd = cd_EctoHurl;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_1 == 5)
    //        {
    //            tempBool = Player->IsEctoSplatCooldown;
    //            ab_cd = Player->EctoSplat_Cooldown;
    //            max_cd = cd_EctoSplat;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_1 == 6)
    //        {
    //            tempBool = Player->IsTornadoCooldown;
    //            ab_cd = Player->Tornado_Cooldown;
    //            max_cd = cd_Tornado;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        //else if (Player->K_1 == 7)
    //        //{

    //        //}
    //        //else if (Player->K_1 == 8)
    //        //{

    //        //}
    //        else if (Player->K_1 == 9)
    //        {
    //            tempBool = Player->IsRattleCooldown;
    //            ab_cd = Player->Rattle_Cooldown;
    //            max_cd = cd_Rattle;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_1 == 10)
    //        {
    //            tempBool = Player->IsTamperCooldown;
    //            ab_cd = Player->Tamper_Cooldown;
    //            max_cd = cd_Tamper;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }

    //    }
    //    // Key ability 2
    //    if (Player->K_2 == i)
    //    {
    //        //tempBool = ReturnAbilityBool();
    //        if (Player->K_2 == 1)
    //        {
    //            tempBool = Player->IsP3Cooldown;
    //            ab_cd = Player->P3_Cooldown;
    //            max_cd = cd_P3;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_2 == 2)
    //        {
    //            tempBool = Player->IsScreechCooldown;
    //            ab_cd = Player->Screech_Cooldown;
    //            max_cd = cd_Screech;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_2 == 3)
    //        {
    //            tempBool = Player->IsP4Cooldown;
    //            ab_cd = Player->P4_Cooldown;
    //            max_cd = cd_P4;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_2 == 4)
    //        {
    //            tempBool = Player->IsEctoHurlCooldown;
    //            ab_cd = Player->EctoHurl_Cooldown;
    //            max_cd = cd_EctoHurl;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_2 == 5)
    //        {
    //            tempBool = Player->IsEctoSplatCooldown;
    //            ab_cd = Player->EctoSplat_Cooldown;
    //            max_cd = cd_EctoSplat;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_2 == 6)
    //        {
    //            tempBool = Player->IsTornadoCooldown;
    //            ab_cd = Player->Tornado_Cooldown;
    //            max_cd = cd_Tornado;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        //else if (Player->K_2 == 7)
    //        //{

    //        //}
    //        //else if (Player->K_2 == 8)
    //        //{

    //        //}
    //        else if (Player->K_2 == 9)
    //        {
    //            tempBool = Player->IsRattleCooldown;
    //            ab_cd = Player->Rattle_Cooldown;
    //            max_cd = cd_Rattle;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_2 == 10)
    //        {
    //            tempBool = Player->IsTamperCooldown;
    //            ab_cd = Player->Tamper_Cooldown;
    //            max_cd = cd_Tamper;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }

    //    }

    //    // Key ability 3
    //    if (Player->K_3 == i)
    //    {
    //        //tempBool = ReturnAbilityBool();
    //        if (Player->K_3 == 1)
    //        {
    //            tempBool = Player->IsP3Cooldown;
    //            ab_cd = Player->P3_Cooldown;
    //            max_cd = cd_P3;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_3 == 2)
    //        {
    //            tempBool = Player->IsScreechCooldown;
    //            ab_cd = Player->Screech_Cooldown;
    //            max_cd = cd_Screech;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_3 == 3)
    //        {
    //            tempBool = Player->IsP4Cooldown;
    //            ab_cd = Player->P4_Cooldown;
    //            max_cd = cd_P4;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_3 == 4)
    //        {
    //            tempBool = Player->IsEctoHurlCooldown;
    //            ab_cd = Player->EctoHurl_Cooldown;
    //            max_cd = cd_EctoHurl;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_3 == 5)
    //        {
    //            tempBool = Player->IsEctoSplatCooldown;
    //            ab_cd = Player->EctoSplat_Cooldown;
    //            max_cd = cd_EctoSplat;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_3 == 6)
    //        {
    //            tempBool = Player->IsTornadoCooldown;
    //            ab_cd = Player->Tornado_Cooldown;
    //            max_cd = cd_Tornado;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        //else if (Player->K_3 == 7)
    //        //{

    //        //}
    //        //else if (Player->K_3 == 8)
    //        //{

    //        //}
    //        else if (Player->K_3 == 9)
    //        {
    //            tempBool = Player->IsRattleCooldown;
    //            ab_cd = Player->Rattle_Cooldown;
    //            max_cd = cd_Rattle;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }
    //        else if (Player->K_3 == 10)
    //        {
    //            tempBool = Player->IsTamperCooldown;
    //            ab_cd = Player->Tamper_Cooldown;
    //            max_cd = cd_Tamper;
    //            if (tempBool)
    //            {
    //                return ab_cd / max_cd;
    //            }
    //        }

    //    }


    // TESTING HERE ===================================================

    for (int i = 1; i <= ab_max; i++)
    {
        if (key == i)
        {
            //tempBool = ReturnAbilityBool();
            if (key == 1)
            {
                tempBool = Player->IsP3Cooldown;
                ab_cd = Player->P3_Cooldown;
                max_cd = Player->cd_P3;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }
            else if (key == 2)
            {
                tempBool = Player->IsScreechCooldown;
                ab_cd = Player->Screech_Cooldown;
                max_cd = Player->cd_Screech;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }
            else if (key == 3)
            {
                tempBool = Player->IsP4Cooldown;
                ab_cd = Player->P4_Cooldown;
                max_cd = Player->cd_P4;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }
            else if (key == 4)
            {
                tempBool = Player->IsEctoHurlCooldown;
                ab_cd = Player->EctoHurl_Cooldown;
                max_cd = Player->cd_EctoHurl;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }
            else if (key == 5)
            {
                tempBool = Player->IsEctoSplatCooldown;
                ab_cd = Player->EctoSplat_Cooldown;
                max_cd = Player->cd_EctoSplat;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }
            else if (key == 6)
            {
                tempBool = Player->IsTornadoCooldown;
                ab_cd = Player->Tornado_Cooldown;
                max_cd = Player->cd_Tornado;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }
            else if (key == 7)
            {
                tempBool = Player->IsColdTouchCooldown;
                ab_cd = Player->ColdTouch_Cooldown;
                max_cd = Player->cd_ColdTouch;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }

            }
            else if (key == 8)
            {
                tempBool = Player->IsKnockCooldown;
                ab_cd = Player->Knock_Cooldown;
                max_cd = Player->cd_Knock;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }
            else if (key == 9)
            {
                tempBool = Player->IsRattleCooldown;
                ab_cd = Player->Rattle_Cooldown;
                max_cd = Player->cd_Rattle;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }
            else if (key == 10)
            {
                tempBool = Player->IsTamperCooldown;
                ab_cd = Player->Tamper_Cooldown;
                max_cd = Player->cd_Tamper;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }
            else if (key == 11) // shiver
            {
                tempBool = Player->IsShiverCooldown; // change
                ab_cd = Player->Shiver_Cooldown;
                max_cd = Player->cd_Shiver;
                if (tempBool)
                {
                    return ab_cd / max_cd;
                }
            }

        }
    }
    return 0.f;
}

int UKiosk::DetermineIcon(int key)
{

    int ab_max = 11;

    for (int i = 1; i <= ab_max; i++)
    {
        if (key == i)
        {
            //tempBool = ReturnAbilityBool();
            if (key == 1)
            {
                return 1;
            }
            else if (key == 2)
            {
                return 2;
            }
            else if (key == 3)
            {
                return 3;
            }
            else if (key == 4)
            {
                return 4;
            }
            else if (key == 5)
            {
                return 5;
            }
            else if (key == 6)
            {
                return 6;
            }
            else if (key == 7)
            {
                return 7;
            }
            else if (key == 8)
            {
                return 8;
            }
            else if (key == 9)
            {
                return 9;
            }
            else if (key == 10)
            {
                return 10;
            }
            else if (key == 11)
            {
                return 11;
            }

        }
    }
    return 0;
}

void UKiosk::PutAbilityToSlot(int AbilityValueToInsert, int AbilityCost, bool AssignAB)
{
    if (Player != nullptr)
    {
        if (Player->K_1 != AbilityValueToInsert && Player->K_2 != AbilityValueToInsert && Player->K_3 != AbilityValueToInsert && Player->K_4 != AbilityValueToInsert)
        {
            if (Player->K_1 == 0)
            {
                KB1 = true;
                KB2 = false;
                KB3 = false;
                KB4 = false;
                if (KB1)
                {
                    KB1 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    Player->K_1 = AbilityValueToInsert;
                    AssignAB = true;
                }

            }
            else if (Player->K_2 == 0)
            {
                KB1 = false;
                KB2 = true;
                KB3 = false;
                KB4 = false;
                if (KB2)
                {
                    KB2 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    Player->K_2 = AbilityValueToInsert;
                    AssignAB = true;
                }
            }
            else if (Player->K_3 == 0)
            {
                KB1 = false;
                KB2 = false;
                KB3 = true;
                KB4 = false;
                if (KB3)
                {
                    KB3 = false;
                    //AbilityKeyToChange = AbilityValueToInsert;
                    Player->K_3 = AbilityValueToInsert;
                    AssignAB = true;
                }
            }
        }
        //else if (Player->K_4 == 0)
        //{
        //    KB1 = false;
        //    KB2 = false;
        //    KB3 = false;
        //    KB4 = true;
        //    if (KB4)
        //    {
        //        KB1 = false;
        //        //AbilityKeyToChange = AbilityValueToInsert;
        //        Player->K_1 = AbilityValueToInsert;
        //        AssignAB = true;
        //    }
        //}
    }

}



