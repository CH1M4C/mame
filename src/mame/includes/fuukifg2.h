// license:BSD-3-Clause
// copyright-holders:Luca Elia,Paul Priest

#include "machine/gen_latch.h"
#include "sound/okim6295.h"
#include "video/fuukifg.h"
#include "emupal.h"
#include "screen.h"

class fuuki16_state : public driver_device
{
public:
	enum
	{
		TIMER_LEVEL_1_INTERRUPT,
		TIMER_VBLANK_INTERRUPT,
		TIMER_RASTER_INTERRUPT
	};

	fuuki16_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_audiocpu(*this, "audiocpu")
		, m_oki(*this, "oki")
		, m_gfxdecode(*this, "gfxdecode")
		, m_screen(*this, "screen")
		, m_palette(*this, "palette")
		, m_fuukivid(*this, "fuukivid")
		, m_soundlatch(*this, "soundlatch")
		, m_vram(*this, "vram.%u", 0)
		, m_vregs(*this, "vregs")
		, m_unknown(*this, "unknown")
		, m_priority(*this, "priority")
		, m_soundbank(*this, "soundbank")
	{ }

	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_audiocpu;
	required_device<okim6295_device> m_oki;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<screen_device> m_screen;
	required_device<palette_device> m_palette;
	required_device<fuukivid_device> m_fuukivid;
	required_device<generic_latch_8_device> m_soundlatch;

	/* memory pointers */
	required_shared_ptr_array<uint16_t,4> m_vram;
	required_shared_ptr<uint16_t> m_vregs;
	required_shared_ptr<uint16_t> m_unknown;
	required_shared_ptr<uint16_t> m_priority;

	required_memory_bank m_soundbank;

	/* video-related */
	tilemap_t     *m_tilemap[4];

	/* misc */
	emu_timer   *m_level_1_interrupt_timer;
	emu_timer   *m_vblank_interrupt_timer;
	emu_timer   *m_raster_interrupt_timer;

	DECLARE_WRITE16_MEMBER(vregs_w);
	DECLARE_WRITE8_MEMBER(sound_command_w);
	DECLARE_WRITE8_MEMBER(sound_rombank_w);
	template<int Layer> DECLARE_WRITE16_MEMBER(vram_w);
	DECLARE_WRITE8_MEMBER(oki_banking_w);

	template<int Layer> TILE_GET_INFO_MEMBER(get_tile_info);

	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;

	uint32_t screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void draw_layer( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int i, int flag, int pri );

	void fuuki16(machine_config &config);
	void fuuki16_map(address_map &map);
	void fuuki16_sound_io_map(address_map &map);
	void fuuki16_sound_map(address_map &map);
protected:
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;
};
