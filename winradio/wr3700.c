/*
 *  Hamlib WiNRADiO backend - WR3700 description
 *  Copyright (c) 2001 by Stephane Fillod
 *
 *		$Id: wr3700.c,v 1.2 2001-07-13 19:08:15 f4cfe Exp $
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <math.h>

#include <hamlib/rig.h>
#include <hamlib/riglist.h>
#include <serial.h>
#include <misc.h>
#include "winradio.h"


/*
 * Winradio rigs capabilities.
 */

#define WR3700_FUNC  RIG_FUNC_FAGC
#define WR3700_SET_LEVEL (RIG_LEVEL_ATT | RIG_LEVEL_AF | RIG_LEVEL_IF | RIG_LEVEL_RF)
#define WR3700_LEVEL (WR3700_SET_LEVEL | RIG_LEVEL_STRENGTH)

#define WR3700_MODES (RIG_MODE_AM | RIG_MODE_CW | \
                     RIG_MODE_USB | RIG_MODE_LSB | RIG_MODE_FM)

const struct rig_caps wr3700_caps = {
  rig_model:     RIG_MODEL_WR3700,
  model_name:    "WR-3700",
  mfg_name:      "Winradio",
  version:       "0.6",
  copyright:	 "LGPL",
  status:        RIG_STATUS_NEW,
  rig_type:      RIG_TYPE_PCRECEIVER,
  port_type:     RIG_PORT_DEVICE,
  targetable_vfo:	 0,
  ptt_type:      RIG_PTT_NONE,
  dcd_type:      RIG_DCD_NONE,
  has_get_func:  WR3700_FUNC,
  has_set_func:  WR3700_FUNC,
  has_get_level: WR3700_LEVEL,
  has_set_level: WR3700_SET_LEVEL,
  has_get_parm:	 RIG_PARM_NONE,
  has_set_parm:	 RIG_PARM_NONE,
  ctcss_list:	 NULL,
  dcs_list:  	 NULL,
  chan_list:	 { RIG_CHAN_END, },
  transceive:    RIG_TRN_OFF,
  max_ifshift:	 kHz(2),
  attenuator:    { 20, RIG_DBLST_END, },
  rx_range_list1: { {start:kHz(150),end:GHz(4),modes:WR3700_MODES,
		    		 low_power:-1,high_power:-1,vfo:RIG_VFO_A},
					{start:MHz(30),end:GHz(4),modes:RIG_MODE_WFM,
		    		 low_power:-1,high_power:-1,vfo:RIG_VFO_A},
		    		RIG_FRNG_END, },
  rx_range_list2: { {start:kHz(150),end:MHz(824),modes:WR3700_MODES,
		    		 low_power:-1,high_power:-1,vfo:RIG_VFO_A},
					{start:MHz(30),end:MHz(824),modes:RIG_MODE_WFM,
		    		 low_power:-1,high_power:-1,vfo:RIG_VFO_A},
					{start:MHz(849),end:MHz(869),
					 modes:WR3700_MODES|RIG_MODE_WFM,
		    		 low_power:-1,high_power:-1,vfo:RIG_VFO_A},
					{start:MHz(894),end:GHz(4),
					 modes:WR3700_MODES|RIG_MODE_WFM,
		    		 low_power:-1,high_power:-1,vfo:RIG_VFO_A},
		    		RIG_FRNG_END, },
  tx_range_list2: { RIG_FRNG_END, },

  tuning_steps: { {RIG_MODE_SSB|RIG_MODE_CW,1}, 
  				  {RIG_MODE_AM|RIG_MODE_FM|RIG_MODE_WFM,10}, RIG_TS_END, },

  filters:      { {RIG_MODE_SSB|RIG_MODE_CW, kHz(2.5)},
                  {RIG_MODE_AM, kHz(6)},
                  {RIG_MODE_FM, kHz(15)},
                  {RIG_MODE_FM, kHz(6)},
                  {RIG_MODE_FM, kHz(50)},
                  {RIG_MODE_WFM, kHz(230)},
                  RIG_FLT_END, },

  priv:			NULL,	/* priv */

  rig_init:    wr_rig_init,

  set_freq:    wr_set_freq,
  get_freq:    wr_get_freq,
  set_mode:    wr_set_mode,
  get_mode:    wr_get_mode,
  
  set_powerstat:  wr_set_powerstat,
  get_powerstat:  wr_get_powerstat,
  set_level:    wr_set_level,
  get_level:    wr_get_level,
  set_func:     wr_set_func,
  get_func:     wr_get_func,

  get_info:     wr_get_info,
};

