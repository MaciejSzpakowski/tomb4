#include "../tomb4/pch.h"
#include "sound.h"
#include "../specific/3dmath.h"
#include "../specific/dxsound.h"
#include "../specific/function_stubs.h"
#include "camera.h"
#include "gameflow.h"
#include "control.h"

#define MINIAUDIO_IMPLEMENTATION
#include "../tomb4/libs/miniaudio.h"

const char* TrackFileNames[112] =
{
	"044_attack_part_i.wav",
	"008_voncroy9a.wav",
	"100_attack_part_ii.wav",
	"010_voncroy10.wav",
	"015_voncroy14.wav",
	"073_secret.wav",
	"109_lyre_01.wav",
	"042_action_part_iv.wav",
	"043_action_part_v.wav",
	"030_voncroy30.wav",
	"012_voncroy11b.wav",
	"011_voncroy11a.wav",
	"063_misc_inc_01.wav",
	"014_voncroy13b.wav",
	"111_charmer.wav",
	"025_voncroy24b.wav",
	"023_voncroy23.wav",
	"006_voncroy7.wav",
	"024_voncroy24a.wav",
	"110_lyre_02.wav",
	"020_voncroy19.wav",
	"034_voncroy34.wav",
	"054_general_part_ii.wav",
	"036_voncroy36.wav",
	"004_voncroy5.wav",
	"035_voncroy35.wav",
	"027_voncroy27.wav",
	"053_general_part_i.wav",
	"022_voncroy22b.wav",
	"028_voncroy28_l11.wav",
	"003_voncroy4.wav",
	"001_voncroy2.wav",
	"041_action_part_iii.wav",
	"057_general_part_v.wav",
	"018_voncroy17.wav",
	"064_misc_inc_02.wav",
	"033_voncroy33.wav",
	"031_voncroy31_l12.wav",
	"032_voncroy32_l13.wav",
	"016_voncroy15.wav",
	"065_misc_inc_03.wav",
	"040_action_part_ii.wav",
	"112_gods_part_iv.wav",
	"029_voncroy29.wav",
	"007_voncroy8.wav",
	"013_voncroy12_13a_lara4.wav",
	"009_voncroy9b_lara3.wav",
	"081_dig.wav",
	"085_intro.wav",
	"071_ominous_part_i.wav",
	"095_phildoor.wav",
	"061_in_the_pyramid_part_i.wav",
	"050_underwater_find_part_i.wav",
	"058_gods_part_i.wav",
	"005_voncroy6_lara2.wav",
	"045_authentic_tr.wav",
	"060_gods_part_iii.wav",
	"055_general_part_iii.wav",
	"059_gods_part_ii.wav",
	"068_mystery_part_ii.wav",
	"076_captain2.wav",
	"019_lara6_voncroy18.wav",
	"002_voncroy3.wav",
	"066_misc_inc_04.wav",
	"067_mystery_part_i.wav",
	"038_a_short_01.wav",
	"088_key.wav",
	"017_voncroy16_lara5.wav",
	"026_vc25_l9_vc26_l10.wav",
	"056_general_part_iv.wav",
	"021_vc20_l7_vc21_l8_vc22a.wav",
	"096_sarcoph.wav",
	"087_jeepb.wav",
	"091_minilib1.wav",
	"086_jeepa.wav",
	"051_egyptian_mood_part_i.wav",
	"078_croywon.wav",
	"092_minilib2.wav",
	"083_horus.wav",
	"049_close_to_the_end_part_ii.wav",
	"037_vc37_l15_vc38.wav",
	"097_scorpion.wav",
	"089_larawon.wav",
	"094_minilib4.wav",
	"098_throne.wav",
	"048_close_to_the_end.wav",
	"070_mystery_part_iv.wav",
	"093_minilib3.wav",
	"072_puzzle_part_i.wav",
	"074_backpack.wav",
	"069_mystery_part_iii.wav",
	"052_egyptian_mood_part_ii.wav",
	"084_inscrip.wav",
	"099_whouse.wav",
	"047_boss_02.wav",
	"080_crypt2.wav",
	"090_libend.wav",
	"046_boss_01.wav",
	"062_jeep_thrills_max.wav",
	"079_crypt1.wav",
	"082_finale.wav",
	"075_captain1.wav",
	"105_a5_battle.wav",
	"077_crocgod.wav",
	"039_tr4_title_q10.wav",
	"108_a8_coastal.wav",
	"107_a7_train+.wav",
	"101_a1_in_dark.wav",
	"102_a2_in_drips.wav",
	"104_a4_weird1.wav",
	"106_a6_out_day.wav",
	"103_a3_out_night.wav"
};

ma_sound bgtrack;
// bg track has data
bool bgtrackInit;
// bg track has data
long curbgtrack;
ma_sound actiontrack;
// action track has data
bool actiontrackInit;
/* what is playing right now */
ma_sound* curTrack;
float trackVolume;
bool trackStopped;
extern long MusicVolume;

struct sampleData
{
	char* data;
	long size;
} samples[256];

struct
{
	bool isInit;
	ma_sound sound;
	ma_decoder decoder;
} channels[32];

ma_engine engine;
SAMPLE_INFO* sample_infos;
SoundSlot LaSlot[32];
short* sample_lut;
long sound_active = 0;

void GetPanVolume(SoundSlot* slot)
{
	long dx, dy, dz, radius, distance, nPan, nVolume;

	if (slot->distance || slot->pos.x || slot->pos.y || slot->pos.z)
	{
		dx = slot->pos.x - camera.pos.x;
		dy = slot->pos.y - camera.pos.y;
		dz = slot->pos.z - camera.pos.z;
		radius = sample_infos[slot->nSampleInfo].radius << 10;

		if (dx < -radius || dx > radius || dy < -radius || dy > radius || dz < -radius || dz > radius)
		{
			slot->distance = 0;
			slot->nPan = 0;
			slot->nVolume = 0;
		}
		else
		{
			distance = SQUARE(dx) + SQUARE(dy) + SQUARE(dz);

			if (distance <= SQUARE(radius))
			{
				if (distance >= 0x100000)
					distance = phd_sqrt(distance) - 1024;
				else
					distance = 0;

				nPan = (CamRot.y << 4) + phd_atan(dz, dx);
				nVolume = slot->OrigVolume;

				if (distance)
					nVolume = (nVolume * (4096 - (phd_sin((distance << 14) / radius) >> 2))) >> 12;

				if (nVolume > 0)
				{
					if (nVolume > 0x7FFF)
						nVolume = 0x7FFF;

					slot->nVolume = nVolume;
					slot->nPan = nPan;
					slot->distance = distance;
				}
				else
				{
					slot->distance = 0;
					slot->nPan = 0;
					slot->nVolume = 0;
				}
			}
			else
			{
				slot->distance = 0;
				slot->nPan = 0;
				slot->nVolume = 0;
			}
		}
	}
}

void StopSoundEffect(long sfx)
{
	long lut;

	if (sound_active)
	{
		lut = sample_lut[sfx];

		for (int i = 0; i < 32; i++)
		{
			if (LaSlot[i].nSampleInfo >= lut && LaSlot[i].nSampleInfo < (lut + ((sample_infos[lut].flags >> 2) & 0xF)))
			{
				S_SoundStopSample(i);
				LaSlot[i].nSampleInfo = -1;
			}
		}
	}
}

bool isChannelPlaying(int n)
{
	if (channels[n].isInit)
	{
		return ma_sound_is_playing(&channels[n].sound);
	}

	return false;
}

int getFreeChannel()
{
	for (int i = 0; i < 32; i++)
	{
		if (!isChannelPlaying(i)) return i;
	}

	return -1;
}

void SOUND_Init()
{
	for (int i = 0; i < 256; i++)
	{
		samples[i].size = 0;
	}

	for (int i = 0; i < 32; i++)
	{
		channels[i].isInit = false;
	}

	bgtrackInit = false;
	actiontrackInit = false;
	curTrack = nullptr;
	curbgtrack = -1;
	trackStopped = true;
	ma_result result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		exit(100);  // Failed to initialize the engine.
	}

	for (int i = 0; i < 32; i++)
	{
		LaSlot[i].nSampleInfo = -1;
	}

	sound_active = 1;
}

void SOUND_Stop()
{
	if (sound_active)
	{
		S_SoundStopAllSamples();

		for (int i = 0; i < 32; i++)
			LaSlot[i].nSampleInfo = -1;
	}
}

long SoundEffect(long sfx, PHD_3DPOS* pos, long flags)
{
	SAMPLE_INFO* info;
	PHD_3DPOS pos2;
	long lut, radius, pan, dx, dy, dz, distance, volume, OrigVolume, pitch, rnd, sample, flag, vol, slot;

	if (sfx == SFX_LARA_NO)
	{
		switch (Gameflow->Language)
		{
		case 1:
			sfx = SFX_LARA_NO_FRENCH;
			break;

		case 2:
		case 3:
		case 4:
			sfx = SFX_LARA_NO;
			break;

		case 6:
			sfx = SFX_LARA_NO_JAPAN;
			break;
		}
	}

	if (!sound_active || !(flags & SFX_ALWAYS) && (flags & SFX_WATER) != (room[camera.pos.room_number].flags & ROOM_UNDERWATER))
		return 0;

	lut = sample_lut[sfx];

	if (lut == -1)
	{
		//empty func call here
		sample_lut[sfx] = -2;
		return 0;
	}

	if (lut == -2)
		return 0;

	info = &sample_infos[lut];

	if (info->randomness)
	{
		if ((GetRandomDraw() & 0xFF) > info->randomness)
			return 0;
	}

	radius = (info->radius + 1) << 10;
	pan = 0;

	if (pos)
	{
		dx = pos->x_pos - camera.pos.x;
		dy = pos->y_pos - camera.pos.y;
		dz = pos->z_pos - camera.pos.z;

		if (dx < -radius || dx > radius || dy < -radius || dy > radius || dz < -radius || dz > radius)
			return 0;

		distance = SQUARE(dx) + SQUARE(dy) + SQUARE(dz);

		if (distance > SQUARE(radius))
			return 0;

		if (distance >= 0x100000)
			distance = phd_sqrt(distance) - 1024;
		else
			distance = 0;

		if (!(info->flags & 0x1000))
			pan = (CamRot.y << 4) + phd_atan(dz, dx);
	}
	else
	{
		distance = 0;
		pos2.x_pos = 0;
		pos2.y_pos = 0;
		pos2.z_pos = 0;
		pos = &pos2;
	}

	volume = info->volume << 6;

	if (info->flags & 0x4000)
		volume -= GetRandomDraw() << 12 >> 15;

	OrigVolume = volume;

	if (distance)
		volume = (volume * (4096 - (phd_sin((distance << 14) / radius) >> 2))) >> 12;

	if (volume <= 0)
		return 0;

	if (volume > 0x7FFF)
		volume = 0x7FFF;

	if (flags & SFX_SETPITCH)
		pitch = (flags >> 8) & 0xFFFFFF;
	else
		pitch = 0x10000;

	pitch += info->pitch << 9;

	if (info->flags & 0x2000)
		pitch += ((6000 * GetRandomDraw()) >> 14) - 6000;

	if (info->number < 0)
		return 0;

	rnd = (info->flags >> 2) & 0xF;

	if (rnd == 1)
		sample = info->number;
	else
		sample = info->number + ((rnd * GetRandomDraw()) >> 15);

	flag = info->flags & 3;

	switch (flag)
	{
	case 1:

		for (int i = 0; i < 32; i++)
		{
			if (LaSlot[i].nSampleInfo == lut)
			{
				if (S_SoundSampleIsPlaying(i))
					return 0;

				LaSlot[i].nSampleInfo = -1;
			}
		}

		break;

	case 2:

		for (int i = 0; i < 32; i++)
		{
			if (LaSlot[i].nSampleInfo == lut)
			{
				S_SoundStopSample(i);
				LaSlot[i].nSampleInfo = -1;
				break;
			}
		}

		break;

	case 3:

		for (int i = 0; i < 32; i++)
		{
			if (LaSlot[i].nSampleInfo == lut)
			{
				if (volume > LaSlot[i].nVolume)
				{
					LaSlot[i].OrigVolume = OrigVolume;
					LaSlot[i].nVolume = volume;
					LaSlot[i].nPan = pan;
					LaSlot[i].nPitch = pitch;
					LaSlot[i].distance = distance;
					LaSlot[i].pos.x = pos->x_pos;
					LaSlot[i].pos.y = pos->y_pos;
					LaSlot[i].pos.z = pos->z_pos;
					return 1;
				}

				return 0;
			}
		}

		break;
	}

	if (flag == 3)
		dx = S_SoundPlaySampleLooped(sample, (ushort)volume, pitch, (short)pan);
	else
		dx = S_SoundPlaySample(sample, (ushort)volume, pitch, (short)pan);

	if (dx >= 0)
	{
		LaSlot[dx].OrigVolume = OrigVolume;
		LaSlot[dx].nVolume = volume;
		LaSlot[dx].nPan = pan;
		LaSlot[dx].nPitch = pitch;
		LaSlot[dx].nSampleInfo = lut;
		LaSlot[dx].distance = distance;
		LaSlot[dx].pos.x = pos->x_pos;
		LaSlot[dx].pos.y = pos->y_pos;
		LaSlot[dx].pos.z = pos->z_pos;
		return 1;
	}

	if (dx == -1)
	{
		vol = 0x8000000;
		slot = -1;

		for (int i = 1; i < 32; i++)
		{
			if ((LaSlot[i].nSampleInfo >= 0) && (LaSlot[i].nVolume <= vol))
			{
				vol = LaSlot[i].nVolume;
				slot = i;
			}
		}

		if (volume > vol)
		{
			S_SoundStopSample(slot);
			LaSlot[slot].nSampleInfo = -1;

			if (flag == 3)
				dx = S_SoundPlaySampleLooped(sample, (ushort)volume, pitch, (short)pan);
			else
				dx = S_SoundPlaySample(sample, (ushort)volume, pitch, (short)pan);

			if (dx >= 0)
			{
				LaSlot[dx].OrigVolume = OrigVolume;
				LaSlot[dx].nVolume = volume;
				LaSlot[dx].nPan = pan;
				LaSlot[dx].nPitch = pitch;
				LaSlot[dx].nSampleInfo = lut;
				LaSlot[dx].distance = distance;
				LaSlot[dx].pos.x = pos->x_pos;
				LaSlot[dx].pos.y = pos->y_pos;
				LaSlot[dx].pos.z = pos->z_pos;
				return 1;
			}
		}

		return 0;
	}

//	if (sample >= 0)
		//empty func call here

	info->number = -1;
	return 0;
}

void SayNo()
{
	long fx;

	fx = SFX_LARA_NO;

	if (Gameflow->Language == FRENCH)
		fx = SFX_LARA_NO_FRENCH;
	else if (Gameflow->Language == JAPAN)
		fx = SFX_LARA_NO_JAPAN;

	SoundEffect(fx, 0, SFX_ALWAYS);
}

void setTrackVolume(long volume)
{

}

void initSample(char* data, long size, long num)
{
	if (samples[num].size)
	{
		delete samples[num].data;
		samples[num].size = 0;
	}

	samples[num].size = size;
	samples[num].data = new char[size];
	memcpy(samples[num].data, data, size);
}

void sampleSetVolume(long num, long volume)
{
}

void sampleSetPitch(long num, long pitch)
{
	if (!channels[num].isInit) return;
	ulong frequency;
	frequency = ulong((float)pitch / 65536.0F * 22050.0F);

	if (frequency < 100)
		frequency = 100;
	else if (frequency > 100000)
		frequency = 100000;

	ma_sound_set_pitch(&channels[num].sound, frequency / 22050.0F);
}

void sampleSetPan(long num, long pan)
{
}

int playSample(long num, long volume, long pitch, long pan, ulong loopcount)
{
	//10 ^ ((volume/100.0f)/20.0f)
	float fvolume = powf(10, ((volume / 100.0f) / 20.0f));
	printf("sample vol:%d pitch:%d pan:%d\n", volume, pitch, pan);
	int c = getFreeChannel();
	if (c == -1) return -1;

	if (channels[c].isInit)
	{
		ma_sound_uninit(&channels[c].sound);
		ma_decoder_uninit(&channels[c].decoder);
	}

	channels[c].isInit = true;
	ma_decoder_init_memory(samples[num].data, samples[num].size, nullptr, &channels[c].decoder);
	ma_sound_init_from_data_source(&engine, &channels[c].decoder, 0, 0, &channels[c].sound);

	if (loopcount)
	{
		ma_sound_set_looping(&channels[c].sound, true);
	}

	ma_sound_set_volume(&channels[c].sound, fvolume);
	sampleSetPitch(c, pitch);
	ma_sound_start(&channels[c].sound);
	return c;
}

void stopSample(long num)
{
	ma_sound_stop(&channels[num].sound);
}

void stopTrack()
{
	trackStopped = true;


	if (curTrack)
	{
		ma_sound_stop(curTrack);
		curTrack = nullptr;
	}
}

void playTrack(long track, long mode)
{
	stopTrack();
	trackStopped = false;

	char filename[256];
	wsprintf(filename, "audio\\%s", TrackFileNames[track]);

	// currently 1 means loop so assumptions is that it's main bg track
	if (mode == 1)
	{
		// bg track changed
		if (curbgtrack != track)
		{
			if (bgtrackInit) ma_sound_uninit(&bgtrack);
			bgtrackInit = false;
			curbgtrack = -1;

			ma_result result = ma_sound_init_from_file(&engine, filename, MA_SOUND_FLAG_NO_SPATIALIZATION| MA_SOUND_FLAG_NO_PITCH, NULL, NULL, &bgtrack);
			if (result != MA_SUCCESS) {
				return;
			}
			bgtrackInit = true;
			curbgtrack = track;
		}

		curTrack = &bgtrack;
		ma_sound_set_looping(curTrack, true);
	}
	else
	{
		if (actiontrackInit) ma_sound_uninit(&actiontrack);
		actiontrackInit = false;

		ma_result result = ma_sound_init_from_file(&engine, filename, MA_SOUND_FLAG_NO_SPATIALIZATION| MA_SOUND_FLAG_NO_PITCH, NULL, NULL, &actiontrack);
		if (result != MA_SUCCESS) {
			return;
		}
		actiontrackInit = true;
		curTrack = &actiontrack;
	}

	ma_sound_seek_to_pcm_frame(curTrack, 0);
	ma_sound_set_volume(curTrack, MusicVolume / 100.0f);
	ma_sound_start(curTrack);
}

// if no track is playing then play bg track
void updateTrack()
{
	if (trackStopped) return;

	if (!curTrack && bgtrackInit)
	{
		ma_sound_seek_to_pcm_frame(&bgtrack, 0);
		curTrack = &bgtrack;
		ma_sound_set_looping(curTrack, true);
		ma_sound_set_volume(curTrack, MusicVolume / 100.0f);
		ma_sound_start(curTrack);
	}
	else if (curTrack && !ma_sound_is_playing(curTrack))
	{
		ma_sound_seek_to_pcm_frame(&bgtrack, 0);
		curTrack = &bgtrack;
		ma_sound_set_looping(curTrack, true);
		ma_sound_set_volume(curTrack, MusicVolume / 100.0f);
		ma_sound_start(curTrack);
	}
}

void setTrackVolume()
{
	if(curTrack)
		ma_sound_set_volume(curTrack, MusicVolume / 100.0f);
}