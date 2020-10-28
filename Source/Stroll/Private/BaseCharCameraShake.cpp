// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharCameraShake.h"

UBaseCharCameraShake::UBaseCharCameraShake()
{
    OscillationDuration = .25f;
    OscillationBlendInTime = .02f;
    OscillationBlendOutTime = .05f;

    RotOscillation.Pitch.Amplitude = FMath::RandRange(1.0f, 3.f);
    RotOscillation.Pitch.Frequency = FMath::RandRange(1.0f, 3.f);

    RotOscillation.Yaw.Amplitude = FMath::RandRange(1.0f, 3.f);
    RotOscillation.Yaw.Frequency = FMath::RandRange(5.0f, 10.f);
}
