// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GetAbilityInfo.h"
#include "GASCharacter.generated.h"

UCLASS()
class GASASSOCIATE_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Add Ability System Component to Character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility")
	class UGASAbilitySystemComponent* AbilitySystemComponent;

	//Add Attribute Set to Character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility")
	const class UGASAttributeSet* AttributeSetVar;

	//Add Variable for Initial Abilities (do not leave blank!)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGameplayAbility")
	TArray<TSubclassOf<class UGameplayAbility>> InitialAbilities;

	//Interface Function to return Ability System Component of this character
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//Initialize a single Ability (Generic Version)
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	//Initialize Ability Multi (Generic Version)
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel);

	//Initialize a single Ability (SERVER Version)
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GASGameplayAbility|Server Version")
	void Server_InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	//Initialize Ability Multi (SERVER Version)
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Server Version")
	void Server_InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel);

	//Get Ability Info from a GA Class, Ability Cooldown and Ability Costs
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	FGameplayAbilityInfo GetAbilityInfoFromAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass, int32 AtAbilityLevel);

	//Server/Client Functions, not exposed to Blueprints
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//********Ability Modifier Functions********
	//These are the Generic Versions that can be called either from Server or Client
	//May not work if triggered from Client in certain situations

	//Remove Abilities with Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void RemoveAbilityWithTags(FGameplayTagContainer TagContainer);

	//Change Ability Level with Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel);

	//Cancel Ability With Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags);

	//Add Loose Gameplay Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void AddLooseGameplayTag(FGameplayTag TagToAdd);

	//Remove Loose Gameplay Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void RemoveLooseGameplayTags(FGameplayTag TagsToRemove);

	//********Ability Modifier Functions SERVER ONLY********
	//These are the Server Only Versions that can be called either from Server or Client
	//They will always be called as server even when called from a Client event

	//Remove Abilities with Tag SERVER
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GASGameplayAbility|Server Version")
	void Server_RemoveAbilityWithTags(FGameplayTagContainer TagContainer);

	//Change Ability Level with Tag SERVER
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GASGameplayAbility|Server Version")
	void Server_ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel);

	//Cancel Ability With Tag SERVER
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GASGameplayAbility|Server Version")
	void Server_CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags);

	//Add Loose Gameplay Tag SERVER
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GASGameplayAbility|Server Version")
	void Server_AddLooseGameplayTag(FGameplayTag TagToAdd);

	//Remove Loose Gameplay Tag SERVER
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GASGameplayAbility|Server Version")
	void Server_RemoveLooseGameplayTags(FGameplayTag TagsToRemove);

	//==PATTERN==
	//Native functions for Attribute Change Delagates. Using UFUNCTION() to expose to BP Side Link

	UFUNCTION()
	virtual void OnHealthChangedNative(float Health, int32 StackCount);
	UFUNCTION()
	virtual void OnManaChangedNative(float Mana, int32 StackCount);
	UFUNCTION()
	virtual void OnMaxHealthChangedNative(float MaxHealth, int32 StackCount);
	UFUNCTION()
	virtual void OnMaxManaChangedNative(float MaxMana, int32 StackCount);
	UFUNCTION()
	virtual void OnMoveSpeedChangedNative(float MoveSpeed, int32 StackCount);


	//******Event that bind to native events and are implemented in BPs********

	//Event Trigger On Health Change
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnHealthChange(float Health, int32 StackCount);
	//Event Trigger On Mana Change
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnManaChange(float Mana, int32 StackCount);
	//Event Trigger On MaxHealth Change
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnMaxHealthChange(float MaxHealth, int32 StackCount);
	//Event Trigger On MaxMana Change
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnMaxManaChange(float MaxMana, int32 StackCount);
	//Event Trigger On MoveSpeed Change
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnMoveSpeedChange(float MoveSpeed, int32 StackCount);


	//*******Ability Values Getter Functions**********

	//Getter for Health Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetHealthValue(float& Health);
	//Getter for Mana Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetManaValue(float& Mana);
	//Getter for MaxHealth Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetMaxHealthValue(float& MaxHealth);
	//Getter for MaxMana Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetMaxManaValue(float& MaxMana);
	//Getter for MoveSpeed Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetMoveSpeedValues(float& MoveSpeed, float& MaxMoveSpeed);


	//*******Ability Values Setter Functions**********

	//Setter for Health Values
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void SetHealthValue(float NewHealth);
	//Setter for Mana Values
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void SetManaValue(float NewMana);
	//Setter for MaxHealth Values
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void SetMaxHealthValue(float NewMaxHealth);
	//Setter for MaxMana Values
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void SetMaxManaValue(float NewMaxMana);
	//Setter for MoveSpeed Values
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void SetMoveSpeedValues(float NewMoveSpeed, float NewMaxMoveSpeed);


};
