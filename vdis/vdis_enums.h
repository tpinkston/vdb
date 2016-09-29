// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#ifndef VDIS_ENUMS_H
#define VDIS_ENUMS_H

#include <ostream>

#include "vdis_enumerations.h"

#include "ack_acknowledge_flag.h"
#include "ack_response_flag.h"
#include "action_seq_dir.h"
#include "action_seq_type.h"
#include "actreq_action_ids.h"
#include "actres_req_status.h"
#include "afapd_version_enum.h"
#include "aggregate_kind.h"
#include "aggregate_state.h"
#include "air_ac_lights.h"
#include "air_eq_slingload.h"
#include "air_form_pat.h"
#include "air_light_mode.h"
#include "air_smoke.h"
#include "amplitude_and_angle.h"
#include "amplitude.h"
#include "angle.h"
#include "antenna_pattern_type.h"
#include "app_ctrl_application_type.h"
#include "app_ctrl_control_type.h"
#include "app_general_status.h"
#include "app_state.h"
#include "areal_minefield_breach.h"
#include "articulated_parts.h"
#include "articulated_parts_metric.h"
#include "attached_parts.h"
#include "beam_function.h"
#include "cf_cat_land.h"
#include "cf_cat_subsurface.h"
#include "cf_cat_surface.h"
#include "cfs_tether.h"
#include "classification_enum.h"
#include "clear_channel.h"
#include "collision.h"
#include "collision_origin.h"
#include "colors.h"
#include "combination.h"
#include "command.h"
#include "comms_type.h"
#include "const_grid.h"
#include "control_type.h"
#include "coord_system.h"
#include "country.h"
#include "crypto_sys.h"
#include "damage_cause.h"
#include "damage_extent.h"
#include "data_rep.h"
#include "datum_ids.h"
#include "dead_reckoning.h"
#include "de_beam_shape.h"
#include "desig_motion_pattern.h"
#include "desig_obj_status.h"
#include "desig_spot_type.h"
#include "desig_system_name.h"
#include "destination_line.h"
#include "destination_type.h"
#include "detonation_result.h"
#include "disguise_status.h"
#include "domain.h"
#include "dr_type.h"
#include "eds_comp_id.h"
#include "eds_dmg_status.h"
#include "eds_surf_dmg.h"
#include "emitter_function.h"
#include "emitter_name.h"
#include "encoding_class.h"
#include "encoding_type.h"
#include "ent_assoc_status.h"
#include "entity_marking.h"
#include "ent_kind.h"
#include "env_cat_air.h"
#include "env_cat_land.h"
#include "env_cat_space.h"
#include "env_cat_subsurface.h"
#include "env_cat_surface.h"
#include "env_geometry_rec_type.h"
#include "env_proc_status.h"
#include "env_state_rec_type.h"
#include "event_type.h"
#include "exercise_state.h"
#include "exp_cat.h"
#include "force_id.h"
#include "formation.h"
#include "frozen_behavior.h"
#include "fuel_meas_units.h"
#include "fuse_burst_desc.h"
#include "grouped_entity.h"
#include "grp_mem_type.h"
#include "headgazweapaim_type.h"
#include "hoist_status.h"
#include "hook_type.h"
#include "ied_presence.h"
#include "iff_system_mode.h"
#include "iff_system_name.h"
#include "iff_system_type.h"
#include "input_source.h"
#include "jamming_technique.h"
#include "land_smoke.h"
#include "laser_function.h"
#include "leaf_coverage.h"
#include "lf_age_group.h"
#include "lf_camouflage_type.h"
#include "lf_cat_air.h"
#include "lf_cat_land.h"
#include "lf_cat_space.h"
#include "lf_cat_subsurface.h"
#include "lf_cat_surface.h"
#include "lf_cloth_scheme.h"
#include "lf_cloth_type.h"
#include "lf_compliance.h"
#include "lf_cond_clean.h"
#include "lf_cond_ext_dmg.h"
#include "lf_cond_mtl.h"
#include "lf_decal_scheme.h"
#include "lf_eq_chute.h"
#include "lf_eq_laser.h"
#include "lf_ethnicity.h"
#include "lf_gender.h"
#include "lf_health.h"
#include "lf_posture.h"
#include "lf_prim_cond_head.h"
#include "lf_sec_cond_facial.h"
#include "lf_weapon_state.h"
#include "major_modulation.h"
#include "message_format_enum.h"
#include "mun_cat.h"
#include "munition_status.h"
#include "nature.h"
#include "object_damage.h"
#include "object_geometry.h"
#include "object_kind.h"
#include "offset_type.h"
#include "on_off.h"
#include "pdu_family.h"
#include "pdu_status_cei.h"
#include "pdu_status_dmi.h"
#include "pdu_status_dti.h"
#include "pdu_status_fti.h"
#include "pdu_status_iai.h"
#include "pdu_status_lvci.h"
#include "pdu_status_rai.h"
#include "pdu_status_tei.h"
#include "pdu_type.h"
#include "phys_assoc_type.h"
#include "phys_conn_type.h"
#include "plat_cat_air.h"
#include "plat_cat_land.h"
#include "plat_cat_space.h"
#include "plat_cat_subsurface.h"
#include "plat_cat_surface.h"
#include "pl_cond_clean.h"
#include "pl_cond_ext_dmg.h"
#include "pl_cond_mtl.h"
#include "pl_cond_rust.h"
#include "pl_decal_scheme.h"
#include "pl_paint_scheme.h"
#include "position.h"
#include "precedence_enum.h"
#include "presence.h"
#include "present_domain.h"
#include "protocol_version.h"
#include "pulse.h"
#include "radar_track.h"
#include "rad_cat.h"
#include "radio_system.h"
#include "radio_type_cat.h"
#include "receiver.h"
#include "record_type.h"
#include "reference_system.h"
#include "repair_response.h"
#include "repair_type.h"
#include "rest_status.h"
#include "rqr_event_type.h"
#include "se_cat.h"
#include "service_type.h"
#include "severity.h"
#include "sf_reason_codes.h"
#include "sling_damage.h"
#include "speed_bump_mtl.h"
#include "start_of_message.h"
#include "station_name.h"
#include "stealth_attach_command.h"
#include "stealth_attach_mode.h"
#include "stealth_spectrum_type.h"
#include "stealthstate_sensor_mode.h"
#include "synchronization_state.h"
#include "tdl_type.h"
#include "terminal_primary.h"
#include "terminal_secondary.h"
#include "transfer_type.h"
#include "transition.h"
#include "transmit_line.h"
#include "transmit_state.h"
#include "ua_function.h"
#include "ua_prop_plant.h"
#include "ua_scan.h"
#include "ua_sys_name.h"
#include "unmodulated.h"
#include "usmtf_version_enum.h"
#include "vmf_version_enum.h"
#include "vp_record_type.h"
#include "warhead_burst_desc.h"
#include "ws_cel_night_ill.h"
#include "ws_cel_season.h"
#include "ws_cel_tod_mode.h"
#include "ws_cel_tod_scene.h"
#include "ws_clouds_dens.h"
#include "ws_clouds_scud_flags.h"
#include "ws_clouds_status.h"
#include "ws_clouds_type.h"
#include "ws_gr_fog_status.h"
#include "ws_haze_status.h"
#include "ws_haze_type.h"
#include "ws_layer_type.h"
#include "ws_light_status.h"
#include "ws_prec_rain.h"
#include "ws_prec_rate.h"
#include "ws_prec_type.h"
#include "ws_thun_status.h"
#include "yes_no.h"

#define ENUM_ACK_ACKNOWLEDGE_FLAG "ACK_ACKNOWLEDGE_FLAG"
#define ENUM_ACK_RESPONSE_FLAG "ACK_RESPONSE_FLAG"
#define ENUM_ACTION_SEQ_DIR "ACTION_SEQ_DIR"
#define ENUM_ACTION_SEQ_TYPE "ACTION_SEQ_TYPE"
#define ENUM_ACTREQ_ACTION_IDS "ACTREQ_ACTION_IDS"
#define ENUM_ACTRES_REQ_STATUS "ACTRES_REQ_STATUS"
#define ENUM_AFAPD_VERSION_ENUM "AFAPD_VERSION_ENUM"
#define ENUM_AGGREGATE_KIND "AGGREGATE_KIND"
#define ENUM_AGGREGATE_STATE "AGGREGATE_STATE"
#define ENUM_AIR_AC_LIGHTS "AIR_AC_LIGHTS"
#define ENUM_AIR_EQ_SLINGLOAD "AIR_EQ_SLINGLOAD"
#define ENUM_AIR_FORM_PAT "AIR_FORM_PAT"
#define ENUM_AIR_LIGHT_MODE "AIR_LIGHT_MODE"
#define ENUM_AIR_SMOKE "AIR_SMOKE"
#define ENUM_AMPLITUDE_AND_ANGLE "AMPLITUDE_AND_ANGLE"
#define ENUM_AMPLITUDE "AMPLITUDE"
#define ENUM_ANGLE "ANGLE"
#define ENUM_ANTENNA_PATTERN_TYPE "ANTENNA_PATTERN_TYPE"
#define ENUM_APP_CTRL_APPLICATION_TYPE "APP_CTRL_APPLICATION_TYPE"
#define ENUM_APP_CTRL_CONTROL_TYPE "APP_CTRL_CONTROL_TYPE"
#define ENUM_APP_GENERAL_STATUS "APP_GENERAL_STATUS"
#define ENUM_APP_STATE "APP_STATE"
#define ENUM_AREAL_MINEFIELD_BREACH "AREAL_MINEFIELD_BREACH"
#define ENUM_ARTICULATED_PARTS "ARTICULATED_PARTS"
#define ENUM_ARTICULATED_PARTS_METRIC "ARTICULATED_PARTS_METRIC"
#define ENUM_ATTACHED_PARTS "ATTACHED_PARTS"
#define ENUM_BEAM_FUNCTION "BEAM_FUNCTION"
#define ENUM_CF_CAT_LAND "CF_CAT_LAND"
#define ENUM_CF_CAT_SUBSURFACE "CF_CAT_SUBSURFACE"
#define ENUM_CF_CAT_SURFACE "CF_CAT_SURFACE"
#define ENUM_CFS_TETHER "CFS_TETHER"
#define ENUM_CLASSIFICATION_ENUM "CLASSIFICATION_ENUM"
#define ENUM_CLEAR_CHANNEL "CLEAR_CHANNEL"
#define ENUM_COLLISION "COLLISION"
#define ENUM_COLLISION_ORIGIN "COLLISION_ORIGIN"
#define ENUM_COLORS "COLORS"
#define ENUM_COMBINATION "COMBINATION"
#define ENUM_COMMAND "COMMAND"
#define ENUM_COMMS_TYPE "COMMS_TYPE"
#define ENUM_CONST_GRID "CONST_GRID"
#define ENUM_CONTROL_TYPE "CONTROL_TYPE"
#define ENUM_COORD_SYSTEM "COORD_SYSTEM"
#define ENUM_COUNTRY "COUNTRY"
#define ENUM_CRYPTO_SYS "CRYPTO_SYS"
#define ENUM_DAMAGE_CAUSE "DAMAGE_CAUSE"
#define ENUM_DAMAGE_EXTENT "DAMAGE_EXTENT"
#define ENUM_DATA_REP "DATA_REP"
#define ENUM_DATUM_IDS "DATUM_IDS"
#define ENUM_DEAD_RECKONING "DEAD_RECKONING"
#define ENUM_DE_BEAM_SHAPE "DE_BEAM_SHAPE"
#define ENUM_DESIG_MOTION_PATTERN "DESIG_MOTION_PATTERN"
#define ENUM_DESIG_OBJ_STATUS "DESIG_OBJ_STATUS"
#define ENUM_DESIG_SPOT_TYPE "DESIG_SPOT_TYPE"
#define ENUM_DESIG_SYSTEM_NAME "DESIG_SYSTEM_NAME"
#define ENUM_DESTINATION_LINE "DESTINATION_LINE"
#define ENUM_DESTINATION_TYPE "DESTINATION_TYPE"
#define ENUM_DETONATION_RESULT "DETONATION_RESULT"
#define ENUM_DISGUISE_STATUS "DISGUISE_STATUS"
#define ENUM_DOMAIN "DOMAIN"
#define ENUM_DR_TYPE "DR_TYPE"
#define ENUM_EDS_COMP_ID "EDS_COMP_ID"
#define ENUM_EDS_DMG_STATUS "EDS_DMG_STATUS"
#define ENUM_EDS_SURF_DMG "EDS_SURF_DMG"
#define ENUM_EMITTER_FUNCTION "EMITTER_FUNCTION"
#define ENUM_EMITTER_NAME "EMITTER_NAME"
#define ENUM_ENCODING_CLASS "ENCODING_CLASS"
#define ENUM_ENCODING_TYPE "ENCODING_TYPE"
#define ENUM_ENT_ASSOC_STATUS "ENT_ASSOC_STATUS"
#define ENUM_ENTITY_MARKING "ENTITY_MARKING"
#define ENUM_ENT_KIND "ENT_KIND"
#define ENUM_ENV_CAT_AIR "ENV_CAT_AIR"
#define ENUM_ENV_CAT_LAND "ENV_CAT_LAND"
#define ENUM_ENV_CAT_SPACE "ENV_CAT_SPACE"
#define ENUM_ENV_CAT_SUBSURFACE "ENV_CAT_SUBSURFACE"
#define ENUM_ENV_CAT_SURFACE "ENV_CAT_SURFACE"
#define ENUM_ENV_GEOMETRY_REC_TYPE "ENV_GEOMETRY_REC_TYPE"
#define ENUM_ENV_PROC_STATUS "ENV_PROC_STATUS"
#define ENUM_ENV_STATE_REC_TYPE "ENV_STATE_REC_TYPE"
#define ENUM_EVENT_TYPE "EVENT_TYPE"
#define ENUM_EXERCISE_STATE "EXERCISE_STATE"
#define ENUM_EXP_CAT "EXP_CAT"
#define ENUM_FORCE_ID "FORCE_ID"
#define ENUM_FORMATION "FORMATION"
#define ENUM_FROZEN_BEHAVIOR "FROZEN_BEHAVIOR"
#define ENUM_FUEL_MEAS_UNITS "FUEL_MEAS_UNITS"
#define ENUM_FUSE_BURST_DESC "FUSE_BURST_DESC"
#define ENUM_GROUPED_ENTITY "GROUPED_ENTITY"
#define ENUM_GRP_MEM_TYPE "GRP_MEM_TYPE"
#define ENUM_HEADGAZWEAPAIM_TYPE "HEADGAZWEAPAIM_TYPE"
#define ENUM_HOIST_STATUS "HOIST_STATUS"
#define ENUM_HOOK_TYPE "HOOK_TYPE"
#define ENUM_IED_PRESENCE "IED_PRESENCE"
#define ENUM_IFF_SYSTEM_MODE "IFF_SYSTEM_MODE"
#define ENUM_IFF_SYSTEM_NAME "IFF_SYSTEM_NAME"
#define ENUM_IFF_SYSTEM_TYPE "IFF_SYSTEM_TYPE"
#define ENUM_INPUT_SOURCE "INPUT_SOURCE"
#define ENUM_JAMMING_TECHNIQUE "JAMMING_TECHNIQUE"
#define ENUM_LAND_SMOKE "LAND_SMOKE"
#define ENUM_LASER_FUNCTION "LASER_FUNCTION"
#define ENUM_LEAF_COVERAGE "LEAF_COVERAGE"
#define ENUM_LF_AGE_GROUP "LF_AGE_GROUP"
#define ENUM_LF_CAMOUFLAGE_TYPE "LF_CAMOUFLAGE_TYPE"
#define ENUM_LF_CAT_AIR "LF_CAT_AIR"
#define ENUM_LF_CAT_LAND "LF_CAT_LAND"
#define ENUM_LF_CAT_SPACE "LF_CAT_SPACE"
#define ENUM_LF_CAT_SUBSURFACE "LF_CAT_SUBSURFACE"
#define ENUM_LF_CAT_SURFACE "LF_CAT_SURFACE"
#define ENUM_LF_CLOTH_SCHEME "LF_CLOTH_SCHEME"
#define ENUM_LF_CLOTH_TYPE "LF_CLOTH_TYPE"
#define ENUM_LF_COMPLIANCE "LF_COMPLIANCE"
#define ENUM_LF_COND_CLEAN "LF_COND_CLEAN"
#define ENUM_LF_COND_EXT_DMG "LF_COND_EXT_DMG"
#define ENUM_LF_COND_MTL "LF_COND_MTL"
#define ENUM_LF_DECAL_SCHEME "LF_DECAL_SCHEME"
#define ENUM_LF_EQ_CHUTE "LF_EQ_CHUTE"
#define ENUM_LF_EQ_LASER "LF_EQ_LASER"
#define ENUM_LF_ETHNICITY "LF_ETHNICITY"
#define ENUM_LF_GENDER "LF_GENDER"
#define ENUM_LF_HEALTH "LF_HEALTH"
#define ENUM_LF_POSTURE "LF_POSTURE"
#define ENUM_LF_PRIM_COND_HEAD "LF_PRIM_COND_HEAD"
#define ENUM_LF_SEC_COND_FACIAL "LF_SEC_COND_FACIAL"
#define ENUM_LF_WEAPON_STATE "LF_WEAPON_STATE"
#define ENUM_MAJOR_MODULATION "MAJOR_MODULATION"
#define ENUM_MESSAGE_FORMAT_ENUM "MESSAGE_FORMAT_ENUM"
#define ENUM_MUN_CAT "MUN_CAT"
#define ENUM_MUNITION_STATUS "MUNITION_STATUS"
#define ENUM_NATURE "NATURE"
#define ENUM_OBJECT_DAMAGE "OBJECT_DAMAGE"
#define ENUM_OBJECT_GEOMETRY "OBJECT_GEOMETRY"
#define ENUM_OBJECT_KIND "OBJECT_KIND"
#define ENUM_OFFSET_TYPE "OFFSET_TYPE"
#define ENUM_ON_OFF "ON_OFF"
#define ENUM_PDU_FAMILY "PDU_FAMILY"
#define ENUM_PDU_STATUS_CEI "PDU_STATUS_CEI"
#define ENUM_PDU_STATUS_DMI "PDU_STATUS_DMI"
#define ENUM_PDU_STATUS_DTI "PDU_STATUS_DTI"
#define ENUM_PDU_STATUS_FTI "PDU_STATUS_FTI"
#define ENUM_PDU_STATUS_IAI "PDU_STATUS_IAI"
#define ENUM_PDU_STATUS_LVCI "PDU_STATUS_LVCI"
#define ENUM_PDU_STATUS_RAI "PDU_STATUS_RAI"
#define ENUM_PDU_STATUS_TEI "PDU_STATUS_TEI"
#define ENUM_PDU_TYPE "PDU_TYPE"
#define ENUM_PHYS_ASSOC_TYPE "PHYS_ASSOC_TYPE"
#define ENUM_PHYS_CONN_TYPE "PHYS_CONN_TYPE"
#define ENUM_PLAT_CAT_AIR "PLAT_CAT_AIR"
#define ENUM_PLAT_CAT_LAND "PLAT_CAT_LAND"
#define ENUM_PLAT_CAT_SPACE "PLAT_CAT_SPACE"
#define ENUM_PLAT_CAT_SUBSURFACE "PLAT_CAT_SUBSURFACE"
#define ENUM_PLAT_CAT_SURFACE "PLAT_CAT_SURFACE"
#define ENUM_PL_COND_CLEAN "PL_COND_CLEAN"
#define ENUM_PL_COND_EXT_DMG "PL_COND_EXT_DMG"
#define ENUM_PL_COND_MTL "PL_COND_MTL"
#define ENUM_PL_COND_RUST "PL_COND_RUST"
#define ENUM_PL_DECAL_SCHEME "PL_DECAL_SCHEME"
#define ENUM_PL_PAINT_SCHEME "PL_PAINT_SCHEME"
#define ENUM_POSITION "POSITION"
#define ENUM_PRECEDENCE_ENUM "PRECEDENCE_ENUM"
#define ENUM_PRESENCE "PRESENCE"
#define ENUM_PRESENT_DOMAIN "PRESENT_DOMAIN"
#define ENUM_PROTOCOL_VERSION "PROTOCOL_VERSION"
#define ENUM_PULSE "PULSE"
#define ENUM_RADAR_TRACK "RADAR_TRACK"
#define ENUM_RAD_CAT "RAD_CAT"
#define ENUM_RADIO_SYSTEM "RADIO_SYSTEM"
#define ENUM_RADIO_TYPE_CAT "RADIO_TYPE_CAT"
#define ENUM_RECEIVER "RECEIVER"
#define ENUM_RECORD_TYPE "RECORD_TYPE"
#define ENUM_REFERENCE_SYSTEM "REFERENCE_SYSTEM"
#define ENUM_REPAIR_RESPONSE "REPAIR_RESPONSE"
#define ENUM_REPAIR_TYPE "REPAIR_TYPE"
#define ENUM_REST_STATUS "REST_STATUS"
#define ENUM_RQR_EVENT_TYPE "RQR_EVENT_TYPE"
#define ENUM_SE_CAT "SE_CAT"
#define ENUM_SERVICE_TYPE "SERVICE_TYPE"
#define ENUM_SEVERITY "SEVERITY"
#define ENUM_SF_REASON_CODES "SF_REASON_CODES"
#define ENUM_SLING_DAMAGE "SLING_DAMAGE"
#define ENUM_SPEED_BUMP_MTL "SPEED_BUMP_MTL"
#define ENUM_START_OF_MESSAGE "START_OF_MESSAGE"
#define ENUM_STATION_NAME "STATION_NAME"
#define ENUM_STEALTH_ATTACH_COMMAND "STEALTH_ATTACH_COMMAND"
#define ENUM_STEALTH_ATTACH_MODE "STEALTH_ATTACH_MODE"
#define ENUM_STEALTH_SPECTRUM_TYPE "STEALTH_SPECTRUM_TYPE"
#define ENUM_STEALTHSTATE_SENSOR_MODE "STEALTHSTATE_SENSOR_MODE"
#define ENUM_SYNCHRONIZATION_STATE "SYNCHRONIZATION_STATE"
#define ENUM_TDL_TYPE "TDL_TYPE"
#define ENUM_TERMINAL_PRIMARY "TERMINAL_PRIMARY"
#define ENUM_TERMINAL_SECONDARY "TERMINAL_SECONDARY"
#define ENUM_TRANSFER_TYPE "TRANSFER_TYPE"
#define ENUM_TRANSITION "TRANSITION"
#define ENUM_TRANSMIT_LINE "TRANSMIT_LINE"
#define ENUM_TRANSMIT_STATE "TRANSMIT_STATE"
#define ENUM_UA_FUNCTION "UA_FUNCTION"
#define ENUM_UA_PROP_PLANT "UA_PROP_PLANT"
#define ENUM_UA_SCAN "UA_SCAN"
#define ENUM_UA_SYS_NAME "UA_SYS_NAME"
#define ENUM_UNMODULATED "UNMODULATED"
#define ENUM_USMTF_VERSION_ENUM "USMTF_VERSION_ENUM"
#define ENUM_VMF_VERSION_ENUM "VMF_VERSION_ENUM"
#define ENUM_VP_RECORD_TYPE "VP_RECORD_TYPE"
#define ENUM_WARHEAD_BURST_DESC "WARHEAD_BURST_DESC"
#define ENUM_WS_CEL_NIGHT_ILL "WS_CEL_NIGHT_ILL"
#define ENUM_WS_CEL_SEASON "WS_CEL_SEASON"
#define ENUM_WS_CEL_TOD_MODE "WS_CEL_TOD_MODE"
#define ENUM_WS_CEL_TOD_SCENE "WS_CEL_TOD_SCENE"
#define ENUM_WS_CLOUDS_DENS "WS_CLOUDS_DENS"
#define ENUM_WS_CLOUDS_SCUD_FLAGS "WS_CLOUDS_SCUD_FLAGS"
#define ENUM_WS_CLOUDS_STATUS "WS_CLOUDS_STATUS"
#define ENUM_WS_CLOUDS_TYPE "WS_CLOUDS_TYPE"
#define ENUM_WS_GR_FOG_STATUS "WS_GR_FOG_STATUS"
#define ENUM_WS_HAZE_STATUS "WS_HAZE_STATUS"
#define ENUM_WS_HAZE_TYPE "WS_HAZE_TYPE"
#define ENUM_WS_LAYER_TYPE "WS_LAYER_TYPE"
#define ENUM_WS_LIGHT_STATUS "WS_LIGHT_STATUS"
#define ENUM_WS_PREC_RAIN "WS_PREC_RAIN"
#define ENUM_WS_PREC_RATE "WS_PREC_RATE"
#define ENUM_WS_PREC_TYPE "WS_PREC_TYPE"
#define ENUM_WS_THUN_STATUS "WS_THUN_STATUS"
#define ENUM_YES_NO "YES_NO"


inline std::ostream &operator<<(std::ostream &stream, vdis::ack_acknowledge_flag_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ACK_ACKNOWLEDGE_FLAG, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ack_response_flag_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ACK_RESPONSE_FLAG, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::action_seq_dir_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ACTION_SEQ_DIR, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::action_seq_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ACTION_SEQ_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::actreq_action_ids_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ACTREQ_ACTION_IDS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::actres_req_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ACTRES_REQ_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::afapd_version_enum_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AFAPD_VERSION_ENUM, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::aggregate_kind_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AGGREGATE_KIND, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::aggregate_state_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AGGREGATE_STATE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::air_ac_lights_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AIR_AC_LIGHTS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::air_eq_slingload_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AIR_EQ_SLINGLOAD, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::air_form_pat_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AIR_FORM_PAT, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::air_light_mode_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AIR_LIGHT_MODE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::air_smoke_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AIR_SMOKE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::amplitude_and_angle_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AMPLITUDE_AND_ANGLE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::amplitude_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AMPLITUDE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::angle_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ANGLE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::antenna_pattern_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ANTENNA_PATTERN_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::app_ctrl_application_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_APP_CTRL_APPLICATION_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::app_ctrl_control_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_APP_CTRL_CONTROL_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::app_general_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_APP_GENERAL_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::app_state_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_APP_STATE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::areal_minefield_breach_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_AREAL_MINEFIELD_BREACH, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::articulated_parts_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ARTICULATED_PARTS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::articulated_parts_metric_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ARTICULATED_PARTS_METRIC, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::attached_parts_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ATTACHED_PARTS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::beam_function_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_BEAM_FUNCTION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::cf_cat_land_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_CF_CAT_LAND, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::cf_cat_subsurface_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_CF_CAT_SUBSURFACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::cf_cat_surface_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_CF_CAT_SURFACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::cfs_tether_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_CFS_TETHER, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::classification_enum_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_CLASSIFICATION_ENUM, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::clear_channel_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_CLEAR_CHANNEL, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::collision_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_COLLISION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::collision_origin_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_COLLISION_ORIGIN, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::colors_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_COLORS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::combination_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_COMBINATION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::command_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_COMMAND, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::comms_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_COMMS_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::const_grid_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_CONST_GRID, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::control_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_CONTROL_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::coord_system_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_COORD_SYSTEM, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::country_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_COUNTRY, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::crypto_sys_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_CRYPTO_SYS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::damage_cause_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DAMAGE_CAUSE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::damage_extent_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DAMAGE_EXTENT, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::data_rep_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DATA_REP, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::datum_ids_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DATUM_IDS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::dead_reckoning_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DEAD_RECKONING, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::de_beam_shape_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DE_BEAM_SHAPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::desig_motion_pattern_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DESIG_MOTION_PATTERN, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::desig_obj_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DESIG_OBJ_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::desig_spot_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DESIG_SPOT_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::desig_system_name_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DESIG_SYSTEM_NAME, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::destination_line_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DESTINATION_LINE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::destination_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DESTINATION_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::detonation_result_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DETONATION_RESULT, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::disguise_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DISGUISE_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::domain_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DOMAIN, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::dr_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_DR_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::eds_comp_id_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_EDS_COMP_ID, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::eds_dmg_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_EDS_DMG_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::eds_surf_dmg_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_EDS_SURF_DMG, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::emitter_function_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_EMITTER_FUNCTION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::emitter_name_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_EMITTER_NAME, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::encoding_class_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENCODING_CLASS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::encoding_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENCODING_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ent_assoc_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENT_ASSOC_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::entity_marking_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENTITY_MARKING, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ent_kind_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENT_KIND, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::env_cat_air_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENV_CAT_AIR, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::env_cat_land_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENV_CAT_LAND, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::env_cat_space_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENV_CAT_SPACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::env_cat_subsurface_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENV_CAT_SUBSURFACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::env_cat_surface_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENV_CAT_SURFACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::env_geometry_rec_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENV_GEOMETRY_REC_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::env_proc_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENV_PROC_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::env_state_rec_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ENV_STATE_REC_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::event_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_EVENT_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::exercise_state_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_EXERCISE_STATE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::exp_cat_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_EXP_CAT, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::force_id_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_FORCE_ID, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::formation_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_FORMATION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::frozen_behavior_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_FROZEN_BEHAVIOR, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::fuel_meas_units_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_FUEL_MEAS_UNITS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::fuse_burst_desc_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_FUSE_BURST_DESC, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::grouped_entity_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_GROUPED_ENTITY, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::grp_mem_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_GRP_MEM_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::headgazweapaim_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_HEADGAZWEAPAIM_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::hoist_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_HOIST_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::hook_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_HOOK_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ied_presence_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_IED_PRESENCE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::iff_system_mode_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_IFF_SYSTEM_MODE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::iff_system_name_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_IFF_SYSTEM_NAME, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::iff_system_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_IFF_SYSTEM_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::input_source_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_INPUT_SOURCE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::jamming_technique_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_JAMMING_TECHNIQUE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::land_smoke_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LAND_SMOKE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::laser_function_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LASER_FUNCTION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::leaf_coverage_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LEAF_COVERAGE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_age_group_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_AGE_GROUP, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_camouflage_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_CAMOUFLAGE_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cat_air_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_CAT_AIR, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cat_land_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_CAT_LAND, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cat_space_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_CAT_SPACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cat_subsurface_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_CAT_SUBSURFACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cat_surface_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_CAT_SURFACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cloth_scheme_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_CLOTH_SCHEME, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cloth_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_CLOTH_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_compliance_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_COMPLIANCE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cond_clean_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_COND_CLEAN, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cond_ext_dmg_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_COND_EXT_DMG, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_cond_mtl_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_COND_MTL, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_decal_scheme_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_DECAL_SCHEME, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_eq_chute_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_EQ_CHUTE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_eq_laser_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_EQ_LASER, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_ethnicity_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_ETHNICITY, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_gender_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_GENDER, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_health_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_HEALTH, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_posture_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_POSTURE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_prim_cond_head_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_PRIM_COND_HEAD, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_sec_cond_facial_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_SEC_COND_FACIAL, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::lf_weapon_state_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_LF_WEAPON_STATE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::major_modulation_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_MAJOR_MODULATION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::message_format_enum_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_MESSAGE_FORMAT_ENUM, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::mun_cat_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_MUN_CAT, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::munition_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_MUNITION_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::nature_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_NATURE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::object_damage_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_OBJECT_DAMAGE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::object_geometry_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_OBJECT_GEOMETRY, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::object_kind_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_OBJECT_KIND, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::offset_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_OFFSET_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::on_off_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_ON_OFF, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_family_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_FAMILY, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_status_cei_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_STATUS_CEI, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_status_dmi_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_STATUS_DMI, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_status_dti_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_STATUS_DTI, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_status_fti_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_STATUS_FTI, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_status_iai_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_STATUS_IAI, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_status_lvci_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_STATUS_LVCI, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_status_rai_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_STATUS_RAI, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_status_tei_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_STATUS_TEI, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pdu_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PDU_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::phys_assoc_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PHYS_ASSOC_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::phys_conn_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PHYS_CONN_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::plat_cat_air_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PLAT_CAT_AIR, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::plat_cat_land_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PLAT_CAT_LAND, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::plat_cat_space_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PLAT_CAT_SPACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::plat_cat_subsurface_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PLAT_CAT_SUBSURFACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::plat_cat_surface_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PLAT_CAT_SURFACE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pl_cond_clean_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PL_COND_CLEAN, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pl_cond_ext_dmg_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PL_COND_EXT_DMG, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pl_cond_mtl_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PL_COND_MTL, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pl_cond_rust_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PL_COND_RUST, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pl_decal_scheme_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PL_DECAL_SCHEME, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pl_paint_scheme_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PL_PAINT_SCHEME, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::position_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_POSITION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::precedence_enum_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PRECEDENCE_ENUM, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::presence_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PRESENCE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::present_domain_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PRESENT_DOMAIN, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::protocol_version_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PROTOCOL_VERSION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::pulse_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_PULSE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::radar_track_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_RADAR_TRACK, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::rad_cat_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_RAD_CAT, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::radio_system_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_RADIO_SYSTEM, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::radio_type_cat_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_RADIO_TYPE_CAT, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::receiver_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_RECEIVER, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::record_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_RECORD_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::reference_system_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_REFERENCE_SYSTEM, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::repair_response_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_REPAIR_RESPONSE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::repair_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_REPAIR_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::rest_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_REST_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::rqr_event_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_RQR_EVENT_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::se_cat_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_SE_CAT, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::service_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_SERVICE_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::severity_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_SEVERITY, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::sf_reason_codes_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_SF_REASON_CODES, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::sling_damage_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_SLING_DAMAGE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::speed_bump_mtl_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_SPEED_BUMP_MTL, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::start_of_message_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_START_OF_MESSAGE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::station_name_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_STATION_NAME, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::stealth_attach_command_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_STEALTH_ATTACH_COMMAND, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::stealth_attach_mode_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_STEALTH_ATTACH_MODE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::stealth_spectrum_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_STEALTH_SPECTRUM_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::stealthstate_sensor_mode_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_STEALTHSTATE_SENSOR_MODE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::synchronization_state_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_SYNCHRONIZATION_STATE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::tdl_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_TDL_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::terminal_primary_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_TERMINAL_PRIMARY, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::terminal_secondary_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_TERMINAL_SECONDARY, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::transfer_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_TRANSFER_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::transition_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_TRANSITION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::transmit_line_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_TRANSMIT_LINE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::transmit_state_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_TRANSMIT_STATE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ua_function_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_UA_FUNCTION, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ua_prop_plant_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_UA_PROP_PLANT, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ua_scan_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_UA_SCAN, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ua_sys_name_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_UA_SYS_NAME, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::unmodulated_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_UNMODULATED, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::usmtf_version_enum_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_USMTF_VERSION_ENUM, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::vmf_version_enum_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_VMF_VERSION_ENUM, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::vp_record_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_VP_RECORD_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::warhead_burst_desc_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WARHEAD_BURST_DESC, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_cel_night_ill_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_CEL_NIGHT_ILL, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_cel_season_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_CEL_SEASON, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_cel_tod_mode_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_CEL_TOD_MODE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_cel_tod_scene_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_CEL_TOD_SCENE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_clouds_dens_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_CLOUDS_DENS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_clouds_scud_flags_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_CLOUDS_SCUD_FLAGS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_clouds_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_CLOUDS_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_clouds_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_CLOUDS_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_gr_fog_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_GR_FOG_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_haze_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_HAZE_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_haze_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_HAZE_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_layer_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_LAYER_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_light_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_LIGHT_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_prec_rain_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_PREC_RAIN, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_prec_rate_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_PREC_RATE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_prec_type_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_PREC_TYPE, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::ws_thun_status_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_WS_THUN_STATUS, value));
}

inline std::ostream &operator<<(std::ostream &stream, vdis::yes_no_e value)
{
    return (stream << vdis::enumerations::get_name(ENUM_YES_NO, value));
}

#endif

