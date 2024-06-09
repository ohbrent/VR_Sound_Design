// Fill out your copyright notice in the Description page of Project Settings.

#include "Misc/FileHelper.h"
#include "Recorder.h"

// Sets default values
ARecorder::ARecorder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARecorder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARecorder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARecorder::SaveAudioToWavFile(const TArray<int32>& AudioData, int32 SampleRate, int32 NumChannels)
{
    struct WavHeader
    {
        char ChunkID[4];
        int32 ChunkSize;
        char Format[4];
        char Subchunk1ID[4];
        int32 Subchunk1Size;
        int16 AudioFormat;
        int16 NumChannels;
        int32 SampleRate;
        int32 ByteRate;
        int16 BlockAlign;
        int16 BitsPerSample;
        char Subchunk2ID[4];
        int32 Subchunk2Size;
    };

    WavHeader Header;
    FMemory::Memzero(&Header, sizeof(WavHeader));

    FMemory::Memcpy(Header.ChunkID, "RIFF", 4);
    Header.ChunkSize = 36 + AudioData.Num() * sizeof(int16);
    FMemory::Memcpy(Header.Format, "WAVE", 4);

    FMemory::Memcpy(Header.Subchunk1ID, "fmt ", 4);
    Header.Subchunk1Size = 16;
    Header.AudioFormat = 1;
    Header.NumChannels = NumChannels;
    Header.SampleRate = SampleRate;
    Header.ByteRate = SampleRate * NumChannels * sizeof(int16);
    Header.BlockAlign = NumChannels * sizeof(int16);
    Header.BitsPerSample = 16;

    FMemory::Memcpy(Header.Subchunk2ID, "data", 4);
    Header.Subchunk2Size = AudioData.Num() * sizeof(int16);

    FString FilePath = FPaths::ProjectDir() + TEXT("CapturedAudio.wav");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    IFileHandle* FileHandle = PlatformFile.OpenWrite(*FilePath);

    if (FileHandle)
    {
        FileHandle->Write(reinterpret_cast<const uint8*>(&Header), sizeof(WavHeader));
        FileHandle->Write(reinterpret_cast<const uint8*>(AudioData.GetData()), AudioData.Num() * sizeof(int16));
        delete FileHandle;
    }
}

