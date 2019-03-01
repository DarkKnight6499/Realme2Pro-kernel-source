/**********************************************************************************
* Copyright (c)  2008-2015  Guangdong OPPO Mobile Comm Corp., Ltd
* VENDOR_EDIT
* Description: Charger IC management module for charger system framework.
*                  Manage all charger IC and define abstarct function flow.
* Version   : 1.0
* Date      : 2015-06-22
* Author    : fanhui@PhoneSW.BSP
*           : Fanhong.Kong@ProDrv.CHG
* ------------------------------ Revision History: --------------------------------
* <version>         <date>              <author>                      <desc>
* Revision 1.0    2015-06-22      fanhui@PhoneSW.BSP          Created for new architecture
* Revision 1.0    2015-06-22      Fanhong.Kong@ProDrv.CHG     Created for new architecture
***********************************************************************************/

#ifndef _OPPO_CHARGER_H_
#define _OPPO_CHARGER_H_

#include <linux/power_supply.h>
#include <linux/workqueue.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0))
#include <linux/wakelock.h>
#endif
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#elif CONFIG_FB
#include <linux/notifier.h>
#include <linux/fb.h>
#ifdef CONFIG_DRM_MSM
#include <linux/msm_drm_notify.h>
#endif
#endif

#ifdef CONFIG_OPPO_CHARGER_MTK
#include <linux/i2c.h>
#ifdef CONFIG_OPPO_CHARGER_6750T
#include <mt-plat/battery_meter.h>
#include <mt-plat/mt_boot.h>
#elif defined(CONFIG_OPPO_CHARGER_MTK6763) || defined(CONFIG_OPPO_CHARGER_MTK6771)
#include <mt-plat/battery_meter.h>
#include <mt-plat/mtk_boot.h>
#else /* CONFIG_OPPO_CHARGER_6750T */
#include <mach/mt_typedefs.h>
#include <mach/mt_boot.h>
#endif /* CONFIG_OPPO_CHARGER_6750T */
#else /* CONFIG_OPPO_CHARGER_MTK */
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/machine.h>
#include <linux/qpnp/qpnp-adc.h>
#include <soc/oppo/boot_mode.h>
#ifdef CONFIG_OPPO_MSM8953N_CHARGER
#include "charger_ic/oppo_battery_msm8953_N.h"
#elif defined CONFIG_OPPO_MSM8953_CHARGER
#include "charger_ic/oppo_battery_msm8953.h"
#elif defined CONFIG_OPPO_MSM8998_CHARGER
#include "charger_ic/oppo_battery_msm8998.h"
#elif defined CONFIG_OPPO_MSM8998O_CHARGER
#include "charger_ic/oppo_battery_msm8998_O.h"
#elif defined CONFIG_OPPO_CHARGE_450_O
#include "charger_ic/oppo_battery_sdm450_O.h"
#else /* CONFIG_OPPO_MSM8953_CHARGER */
#include "charger_ic/oppo_battery_msm8976.h"
#endif /* CONFIG_OPPO_MSM8953_CHARGER */

#endif /* CONFIG_OPPO_CHARGER_MTK */
#ifdef CONFIG_OPPO_CHARGER_MTK6771
#include "charger_ic/smb1351-charger.h"
#endif
#define CHG_LOG_CRTI 1
#define CHG_LOG_FULL 2


#define OPCHG_INPUT_CURRENT_LIMIT_CHARGER_MA        2000
#define OPCHG_INPUT_CURRENT_LIMIT_USB_MA            500
#define OPCHG_INPUT_CURRENT_LIMIT_LED_MA            1200
#define OPCHG_INPUT_CURRENT_LIMIT_CAMERA_MA         1000
#define OPCHG_INPUT_CURRENT_LIMIT_CALLING_MA        1200

#define OPCHG_FAST_CHG_MAX_MA                       2000

#define FEATURE_PRINT_CHGR_LOG
#define FEATURE_PRINT_BAT_LOG
#define FEATURE_PRINT_GAUGE_LOG
#define FEATURE_PRINT_STATUS_LOG
/*#define FEATURE_PRINT_OTHER_LOG*/
#define FEATURE_PRINT_VOTE_LOG
#define FEATURE_PRINT_ICHGING_LOG
#define FEATURE_VBAT_PROTECT

#define NOTIFY_CHARGER_OVER_VOL                     1
#define NOTIFY_CHARGER_LOW_VOL                      2
#define NOTIFY_BAT_OVER_TEMP                        3
#define NOTIFY_BAT_LOW_TEMP                         4
#define NOTIFY_BAT_NOT_CONNECT                      5
#define NOTIFY_BAT_OVER_VOL                         6
#define NOTIFY_BAT_FULL                             7
#define NOTIFY_CHGING_CURRENT                       8
#define NOTIFY_CHGING_OVERTIME                      9
#define NOTIFY_BAT_FULL_PRE_HIGH_TEMP               10
#define NOTIFY_BAT_FULL_PRE_LOW_TEMP                11
#define NOTIFY_BAT_FULL_THIRD_BATTERY               14

#define chg_debug(fmt, ...) \
        printk(KERN_ERR "[OPPO_CHG][%s]"fmt, __func__, ##__VA_ARGS__)

#define chg_err(fmt, ...) \
        printk(KERN_ERR "[OPPO_CHG][ERROR][%s]"fmt, __func__, ##__VA_ARGS__)

typedef enum {
        CHG_NONE = 0,
        CHG_DISABLE,
        CHG_SUSPEND,
}OPPO_CHG_DISABLE_STATUS;

typedef enum {
        CHG_STOP_VOTER_NONE                                =        0,
        CHG_STOP_VOTER__BATTTEMP_ABNORMAL                  =        (1 << 0),
        CHG_STOP_VOTER__VCHG_ABNORMAL                      =        (1 << 1),
        CHG_STOP_VOTER__VBAT_TOO_HIGH                      =        (1 << 2),
        CHG_STOP_VOTER__MAX_CHGING_TIME                    =        (1 << 3),
        CHG_STOP_VOTER__FULL                               =        (1 << 4),
}OPPO_CHG_STOP_VOTER;

typedef enum {
        CHARGER_STATUS__GOOD,
        CHARGER_STATUS__VOL_HIGH,
        CHARGER_STATUS__VOL_LOW,
        CHARGER_STATUS__INVALID
}OPPO_CHG_VCHG_STATUS;

typedef enum {
        BATTERY_STATUS__NORMAL = 0,                      /*16C~45C*/
        BATTERY_STATUS__REMOVED,                         /*<-20C*/
        BATTERY_STATUS__LOW_TEMP,                        /*<-3C*/
        BATTERY_STATUS__HIGH_TEMP,                       /*>55C*/
        BATTERY_STATUS__COLD_TEMP,                       /*-3C~0C*/
        BATTERY_STATUS__LITTLE_COLD_TEMP,                /*0C~5C*/
        BATTERY_STATUS__COOL_TEMP,                       /*5C~12C*/
        BATTERY_STATUS__LITTLE_COOL_TEMP,                /*12C~16C*/
        BATTERY_STATUS__WARM_TEMP,                       /*45C~55C*/
        BATTERY_STATUS__INVALID
}
OPPO_CHG_TBATT_STATUS;

typedef enum {
        CRITICAL_LOG_NORMAL = 0,
        CRITICAL_LOG_UNABLE_CHARGING,
        CRITICAL_LOG_BATTTEMP_ABNORMAL,
        CRITICAL_LOG_VCHG_ABNORMAL,
        CRITICAL_LOG_VBAT_TOO_HIGH,
        CRITICAL_LOG_CHARGING_OVER_TIME,
        CRITICAL_LOG_VOOC_WATCHDOG,
        CRITICAL_LOG_VOOC_BAD_CONNECTED,
        CRITICAL_LOG_VOOC_BTB
}OPPO_CHG_CRITICAL_LOG;

typedef enum {
        CHARGING_STATUS_CCCV                =        0X01,
        CHARGING_STATUS_FULL                =        0X02,
        CHARGING_STATUS_FAIL                =        0X03,
}OPPO_CHG_CHARGING_STATUS;

struct tbatt_anti_shake{
        int cold_bound;
        int little_cold_bound;
        int cool_bound;
        int little_cool_bound;
        int normal_bound;
        int warm_bound;
        int hot_bound;
        int overtemp_bound;
};

struct oppo_chg_limits {
        int input_current_charger_ma;
        int input_current_usb_ma;
        int input_current_led_ma;
        int input_current_led_ma_forcmcc;
        int input_current_led_ma_overtemp;
        int input_current_camera_ma;
        int input_current_calling_ma;

        int iterm_ma;
        bool iterm_disabled;
        int recharge_mv;

        int removed_bat_decidegc; /*-19C*/

        int cold_bat_decidegc;  /*-3C*/
        int temp_cold_vfloat_mv;
        int temp_cold_fastchg_current_ma;

        int little_cold_bat_decidegc;  /*0C*/
        int temp_little_cold_vfloat_mv;
        int temp_little_cold_fastchg_current_ma;

        int cool_bat_decidegc;        /*5C*/
        int temp_cool_vfloat_mv;
        int temp_cool_fastchg_current_ma_high;
        int temp_cool_fastchg_current_ma_low;

        int little_cool_bat_decidegc;        /*12C*/
        int temp_little_cool_vfloat_mv;
        int temp_little_cool_fastchg_current_ma;

        int normal_bat_decidegc;        /*16C*/
        int temp_normal_fastchg_current_ma;
        int temp_normal_vfloat_mv_normalchg;

        int warm_bat_decidegc;                /*45C*/
        int temp_warm_vfloat_mv;
        int temp_warm_fastchg_current_ma;

        int hot_bat_decidegc;                /*53C*/
        int non_standard_vfloat_mv;
        int non_standard_fastchg_current_ma;
        int max_chg_time_sec;
        int charger_hv_thr;
        int charger_lv_thr;
        int vbatt_full_thr;
        int vbatt_hv_thr;

        int vfloat_step_mv;
        int vfloat_sw_set;
        int vfloat_over_counts;

        int non_standard_vfloat_sw_limit;
        int cold_vfloat_sw_limit;
        int little_cold_vfloat_sw_limit;
        int cool_vfloat_sw_limit;
        int little_cool_vfloat_sw_limit;
        int normal_vfloat_sw_limit;
        int warm_vfloat_sw_limit;

        int overtemp_bat_decidegc;                /*35C*/

        bool sw_vfloat_over_protect_enable;
        int non_standard_vfloat_over_sw_limit;
        int cold_vfloat_over_sw_limit;
        int little_cold_vfloat_over_sw_limit;
        int cool_vfloat_over_sw_limit;
        int little_cool_vfloat_over_sw_limit;
        int normal_vfloat_over_sw_limit;
        int warm_vfloat_over_sw_limit;

};

struct battery_data{
        int  BAT_STATUS;
        int  BAT_HEALTH;
        int  BAT_PRESENT;
        int  BAT_TECHNOLOGY;
        int  BAT_CAPACITY;
    /* Add for Battery Service*/
        int  BAT_batt_vol;
        int  BAT_batt_temp;

    /* Add for EM */
        int  BAT_TemperatureR;
        int  BAT_TempBattVoltage;
        int  BAT_InstatVolt;
        int  BAT_BatteryAverageCurrent;
        int  BAT_BatterySenseVoltage;
        int  BAT_ISenseVoltage;
        int  BAT_ChargerVoltage;
        int  battery_request_poweroff;//low battery in sleep
        int  fastcharger;
        int  charge_technology;
    /* Dual battery */
        int  BAT_MMI_CHG;//for MMI_CHG_TEST
        int  BAT_FCC;
        int  BAT_SOH;
        int  BAT_CC;
};

struct normalchg_gpio_pinctrl {
        int     chargerid_switch_gpio;
        int     usbid_gpio;
        int     usbid_irq;
        int     ship_gpio;
        struct pinctrl       *pinctrl;
        struct pinctrl_state *chargerid_switch_active;
        struct pinctrl_state *chargerid_switch_sleep;
        struct pinctrl_state *chargerid_switch_default;
        struct pinctrl_state *usbid_active;
        struct pinctrl_state *usbid_sleep;
        struct pinctrl_state *ship_active;
        struct pinctrl_state *ship_sleep;
        struct pinctrl_state *charger_gpio_as_output1;
        struct pinctrl_state *charger_gpio_as_output2;
};
struct usb_shell_ctrl_gpio_pinctrl {
        int                     usb_shell_gpio;
        struct pinctrl          *pinctrl;
        struct pinctrl_state    *usb_shell_ctrl_active;
        struct pinctrl_state    *usb_shell_ctrl_sleep;
        struct pinctrl_state    *usb_shell_ctrl_default;
};

struct oppo_chg_chip {
        struct i2c_client       *client;
        struct device           *dev;
        const struct oppo_chg_operations *chg_ops;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
        struct power_supply        *ac_psy;
#else
        struct power_supply        ac_psy;
#endif

#if defined(CONFIG_OPPO_CHARGER_MTK6763) || defined(CONFIG_OPPO_CHARGER_MTK6771)
        struct power_supply_desc        ac_psd;
        struct power_supply_config      ac_cfg;
        struct power_supply_desc        usb_psd;
        struct power_supply_config      usb_cfg;
        struct power_supply_desc        battery_psd;
#endif
#ifdef CONFIG_OPPO_CHARGER_MTK
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
        struct power_supply     *usb_psy;
#else
        struct power_supply     usb_psy;
#endif
#else
        struct power_supply     *usb_psy;
        struct qcom_pmic        pmic_spmi;
#endif
#ifdef CONFIG_OPPO_CHARGER_MTK6771
        struct smb1351_charger  smb1351;
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
        struct power_supply     *batt_psy;
#else
        struct power_supply     batt_psy;
#endif
/*        struct battery_data battery_main        */
        struct delayed_work     update_work;
        struct delayed_work     mmi_adapter_in_work;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0))
        struct wake_lock        suspend_lock;
#else
        struct wakeup_source    *suspend_ws;
#endif
        atomic_t                charger_suspended;

        struct oppo_chg_limits    limits;
        struct tbatt_anti_shake   anti_shake_bound;
        int                       usb_switch_gpio;

        bool          charger_exist;
        int           charger_type;
        int           charger_volt;
        int           charger_volt_pre;
        int           chg_pretype;

        int           temperature;
        int           batt_volt;
        int           icharging;
        int           soc;
        int           ui_soc;
        int           soc_load;
        bool          authenticate;
        int           hw_aicl_point;
        int           sw_aicl_point;
        int           batt_fcc;
        int           batt_cc;
        int           batt_soh;
        int           batt_rm;
        int           batt_capacity_mah;
        int           tbatt_pre_shake;

        bool          batt_exist;
        bool          batt_full;
        bool          chging_on;
        bool          in_rechging;
        int           charging_state;
        int           total_time;
        unsigned long sleep_tm_sec;

        bool          vbatt_over;
        bool          chging_over_time;
        int           vchg_status;
        int           tbatt_status;
        int           prop_status;
        int           stop_voter;
        int           notify_code;
        int           notify_flag;
        int           request_power_off;

        bool          led_on;
        bool          led_status_change;
        bool          camera_on;
        bool          calling_on;

        bool          ac_online;
#ifdef         CONFIG_OPPO_CHARGER_MTK
        bool          usb_online;
        bool          otg_online;
#endif
        bool          usb_shell_ctrl;
        bool          otg_switch;
        int           mmi_chg;
        int           stop_chg;
        int           mmi_fastchg;
        int           boot_reason;
        int           boot_mode;
        bool          suspend_after_full;
        bool          check_batt_full_by_sw;
        bool          external_gauge;
        bool          chg_ctrl_by_lcd;
        bool          chg_ctrl_by_camera;
        bool          bq25890h_flag;
        bool          chg_ctrl_by_calling;
        bool          fg_bcl_poll;
#ifdef CONFIG_HAS_EARLYSUSPEND
        struct early_suspend chg_early_suspend;
#elif CONFIG_FB
        struct notifier_block chg_fb_notify;
#endif
        struct normalchg_gpio_pinctrl           normalchg_gpio;
        struct usb_shell_ctrl_gpio_pinctrl      usb_shell_ctrl_gpio;
        int                 chargerid_volt;
        bool                chargerid_volt_got;
        int                 enable_shipmode;
/*add for bq24190 charger_type detect*/
        bool                usb_enum_enable;
        struct hrtimer      usb_enum_hrtimer;
        int                 usb_enum_type;
        bool                usb_enum_timeout;
        bool                overtemp_status;
        struct hrtimer      usb_distinguish_hrtimer;
        int                 hrtimer_flag;
};
enum {
        USB_ENUM_DISABLE = 0,
        USB_ENUM_DEFAULT,
        USB_ENUM_DETECTED,
        USB_ENUM_TIMEOUT,
        USB_ENUM_USB_TYPE,
};
#ifdef CONFIG_OPPO_CHARGER_MTK6771
struct smb_irq_info {
        const char              *name;
        int (*smb_irq)(struct oppo_chg_chip *chip, u8 rt_stat);
        int                     high;
        int                     low;
};

struct irq_handler_info {
        u8                      stat_reg;
        u8                      val;
        u8                      prev_val;
        struct smb_irq_info     irq_info[4];
};
#endif
struct oppo_chg_operations {
#if defined(CONFIG_OPPO_CHARGER_MTK6763) || defined(CONFIG_OPPO_CHARGER_MTK6771)
        int (*dump_registers) (struct oppo_chg_chip *chip);
#else
        void (*dump_registers)(struct oppo_chg_chip *chip);
#endif
        void (*disable_adjust_fv)(void);
        int (*kick_wdt)(struct oppo_chg_chip *chip);
        int (*hardware_init)(struct oppo_chg_chip *chip);
        int (*charging_current_write_fast)(struct oppo_chg_chip *chip, int cur);
        void (*set_aicl_point)(struct oppo_chg_chip *chip, int vbatt);
        int (*input_current_write)(struct oppo_chg_chip *chip, int cur);
        int (*float_voltage_write)(struct oppo_chg_chip *chip, int cur);
        int (*term_current_set)(struct oppo_chg_chip *chip, int cur);
        int (*charging_enable)(struct oppo_chg_chip *chip);
        int (*charging_disable)(struct oppo_chg_chip *chip);
        int (*get_charging_enable)(struct oppo_chg_chip *chip);
        int (*charger_suspend)(struct oppo_chg_chip *chip);
        int (*charger_unsuspend)(struct oppo_chg_chip *chip);
        int (*set_rechg_vol)(struct oppo_chg_chip *chip, int vol);
        int (*reset_charger)(struct oppo_chg_chip *chip);
        int (*read_full)(struct oppo_chg_chip *chip);
        int (*otg_enable)(void);
        int (*otg_disable)(void);
        int (*set_charging_term_disable)(struct oppo_chg_chip *chip);
        bool (*check_charger_resume)(struct oppo_chg_chip *chip);

        int (*get_charger_type)(void);
#ifdef CONFIG_OPPO_CHARGER_MTK
        int (*get_chg_pretype)(void);
#endif
        int (*get_charger_volt)(void);
        int (*get_chargerid_volt)(struct oppo_chg_chip *chip);
        void (*set_chargerid_switch_val)(struct oppo_chg_chip *chip, int value);
        int (*get_chargerid_switch_val)(struct oppo_chg_chip *chip);
        void (*set_usb_shell_ctrl_val) (struct oppo_chg_chip *chip, int value);
        int (*get_usb_shell_ctrl_val) (struct oppo_chg_chip *chip);
        bool (*check_chrdet_status)(void);
        int (*get_boot_mode)(void);
        int (*get_boot_reason)(void);
#ifdef CONFIG_OPPO_CHARGER_MTK
        #if !defined CONFIG_OPPO_CHARGER_MTK6763 && !defined CONFIG_OPPO_CHARGER_MTK6771
        int (*get_instant_vbatt)(kal_bool);
#else
        int (*get_instant_vbatt)(void);
#endif
#else
        int (*get_instant_vbatt)(void);
#endif
        int (*get_rtc_soc)(void);
        int (*set_rtc_soc)(int val);
        void (*set_power_off)(void);
        void (*usb_connect)(void);
        void (*usb_disconnect)(void);
#ifndef CONFIG_OPPO_CHARGER_MTK
        int (*get_aicl_ma)(struct oppo_chg_chip *chip);
        void(*rerun_aicl)(struct oppo_chg_chip *chip);
        int (*tlim_en)(struct oppo_chg_chip *chip, bool);
        int (*set_system_temp_level)(struct oppo_chg_chip *chip, int);
        int(*otg_pulse_skip_disable)(struct oppo_chg_chip *chip, enum skip_reason, bool);
        int(*set_dp_dm)(struct oppo_chg_chip *chip, int);
        int(*calc_flash_current)(struct oppo_chg_chip *chip);
#endif
        int (*get_chg_current_step)(struct oppo_chg_chip *chip);
        bool (*need_to_check_ibatt)(struct oppo_chg_chip *chip);
#ifdef CONFIG_OPPO_RTC_DET_SUPPORT
        int (*check_rtc_reset)(void);
#endif /* CONFIG_OPPO_RTC_DET_SUPPORT */
        int (*get_dyna_aicl_result) (struct oppo_chg_chip *chip);
        void (*check_is_iindpm_mode) (struct oppo_chg_chip *chip);
};

/*********************************************
 * oppo_chg_init - initialize oppo_chg_chip
 * @chip: pointer to the oppo_chg_cip
 * @clinet: i2c client of the chip
 *
 * Returns: 0 - success; -1/errno - failed
 **********************************************/
int oppo_chg_parse_dt(struct oppo_chg_chip *chip);

int oppo_chg_init(struct oppo_chg_chip *chip);
void oppo_charger_detect_check(struct oppo_chg_chip *chip);
int oppo_chg_get_prop_batt_health(struct oppo_chg_chip *chip);
void oppo_chg_disable_adjust_fv(void);
bool oppo_chg_wake_update_work(void);
void oppo_chg_soc_update_when_resume(unsigned long sleep_tm_sec);
void oppo_chg_soc_update(void);
int oppo_chg_get_batt_volt(void);

int oppo_chg_get_ui_soc(void);
int oppo_chg_get_soc(void);
int oppo_chg_get_chg_temperature(void);

void oppo_chg_kick_wdt(void);
void oppo_chg_disable_charge(void);
void oppo_chg_unsuspend_charger(void);

int oppo_chg_get_chg_type(void);

int oppo_chg_get_notify_flag(void);

bool get_otg_switch(void);

#ifdef CONFIG_OPPO_CHARGER_MTK
bool oppo_chg_get_otg_online(void);
void oppo_chg_set_otg_online(bool online);
#endif

bool oppo_chg_get_batt_full(void);
bool oppo_chg_get_rechging_status(void);

bool oppo_chg_check_chip_is_null(void);
void oppo_chg_set_charger_type_unknown(void);
int oppo_chg_get_charger_voltage(void);
void oppo_chg_set_chargerid_switch_val(int value);
void oppo_set_usb_shell_ctrl(int value);
int oppo_get_usb_shell_ctrl(void);
void oppo_chg_turn_on_charging(struct oppo_chg_chip *chip);

void oppo_chg_clear_chargerid_info(void);
#ifndef CONFIG_OPPO_CHARGER_MTK
void oppo_chg_variables_reset(struct oppo_chg_chip *chip, bool in);
void oppo_chg_external_power_changed(struct power_supply *psy);
#endif
int oppo_is_rf_ftm_mode(void);

#endif /*_OPPO_CHARGER_H_*/
