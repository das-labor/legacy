/* Copyright 2001 Matej Pfajfar.
 * Copyright 2001-2004 Roger Dingledine.
 * Copyright 2004-2005 Roger Dingledine, Nick Mathewson. */
/* See LICENSE for licensing information */
/* $Id: config.c,v 1.492 2006/01/11 20:09:37 nickm Exp $ */
const char config_c_id[] = \
  "$Id: config.c,v 1.492 2006/01/11 20:09:37 nickm Exp $";

/**
 * \file config.c
 * \brief Code to parse and interpret configuration files.
 **/

#include "or.h"
#ifdef MS_WINDOWS
#include <shlobj.h>
#endif
#include "../common/aes.h"

/** Enumeration of types which option values can take */
typedef enum config_type_t {
  CONFIG_TYPE_STRING = 0,   /**< An arbitrary string. */
  CONFIG_TYPE_UINT,         /**< A non-negative integer less than MAX_INT */
  CONFIG_TYPE_INTERVAL,     /**< A number of seconds, with optional units*/
  CONFIG_TYPE_MEMUNIT,      /**< A number of bytes, with optional units*/
  CONFIG_TYPE_DOUBLE,       /**< A floating-point value */
  CONFIG_TYPE_BOOL,         /**< A boolean value, expressed as 0 or 1. */
  CONFIG_TYPE_ISOTIME,      /**< An ISO-formated time relative to GMT. */
  CONFIG_TYPE_CSV,          /**< A list of strings, separated by commas and
                              * optional whitespace. */
  CONFIG_TYPE_LINELIST,     /**< Uninterpreted config lines */
  CONFIG_TYPE_LINELIST_S,   /**< Uninterpreted, context-sensitive config lines,
                             * mixed with other keywords. */
  CONFIG_TYPE_LINELIST_V,   /**< Catch-all "virtual" option to summarize
                             * context-sensitive config lines when fetching.
                             */
  CONFIG_TYPE_OBSOLETE,     /**< Obsolete (ignored) option. */
} config_type_t;

/** An abbreviation for a configuration option allowed on the command line. */
typedef struct config_abbrev_t {
  const char *abbreviated;
  const char *full;
  int commandline_only;
  int warn;
} config_abbrev_t;

/* Handy macro for declaring "In the config file or on the command line,
 * you can abbreviate <b>tok</b>s as <b>tok</b>". */
#define PLURAL(tok) { #tok, #tok "s", 0, 0 }

/* A list of command-line abbreviations. */
static config_abbrev_t _option_abbrevs[] = {
  PLURAL(ExitNode),
  PLURAL(EntryNode),
  PLURAL(ExcludeNode),
  PLURAL(FirewallPort),
  PLURAL(LongLivedPort),
  PLURAL(HiddenServiceNode),
  PLURAL(HiddenServiceExcludeNode),
  PLURAL(NumCpu),
  PLURAL(RendNode),
  PLURAL(RendExcludeNode),
  PLURAL(StrictEntryNode),
  PLURAL(StrictExitNode),
  { "l", "Log", 1, 0},
  { "BandwidthRateBytes", "BandwidthRate", 0, 0},
  { "BandwidthBurstBytes", "BandwidthBurst", 0, 0},
  { "DirFetchPostPeriod", "StatusFetchPeriod", 0, 0},
  { "MaxConn", "ConnLimit", 0, 1},
  { "ORBindAddress", "ORListenAddress", 0, 0},
  { "DirBindAddress", "DirListenAddress", 0, 0},
  { "SocksBindAddress", "SocksListenAddress", 0, 0},
  { "UseHelperNodes", "UseEntryGuards", 0, 0},
  { "NumHelperNodes", "NumEntryGuards", 0, 0},
  { "UseEntryNodes", "UseEntryGuards", 0, 0},
  { "NumEntryNodes", "NumEntryGuards", 0, 0},
  { NULL, NULL, 0, 0},
};
/* A list of state-file abbreviations, for compatibility. */
static config_abbrev_t _state_abbrevs[] = {
  { "AccountingBytesReadInterval", "AccountingBytesReadInInterval", 0, 0 },
  { "HelperNode", "EntryGuard", 0, 0 },
  { "HelperNodeDownSince", "EntryGuardDownSince", 0, 0 },
  { "HelperNodeUnlistedSince", "EntryGuardUnlistedSince", 0, 0 },
  { "EntryNode", "EntryGuard", 0, 0 },
  { "EntryNodeDownSince", "EntryGuardDownSince", 0, 0 },
  { "EntryNodeUnlistedSince", "EntryGuardUnlistedSince", 0, 0 },
  { NULL, NULL, 0, 0},
};
#undef PLURAL

/** A variable allowed in the configuration file or on the command line. */
typedef struct config_var_t {
  const char *name; /**< The full keyword (case insensitive). */
  config_type_t type; /**< How to interpret the type and turn it into a
                       * value. */
  off_t var_offset; /**< Offset of the corresponding member of or_options_t. */
  const char *initvalue; /**< String (or null) describing initial value. */
} config_var_t;

/** Return the offset of <b>member</b> within the type <b>tp</b>, in bytes */
#define STRUCT_OFFSET(tp, member) \
  ((off_t) (((char*)&((tp*)0)->member)-(char*)0))
/** An entry for config_vars: "The option <b>name</b> has type
 * CONFIG_TYPE_<b>conftype</b>, and corresponds to
 * or_options_t.<b>member</b>"
 */
#define VAR(name,conftype,member,initvalue)                             \
  { name, CONFIG_TYPE_ ## conftype, STRUCT_OFFSET(or_options_t, member), \
      initvalue }
/** An entry for config_vars: "The option <b>name</b> is obsolete." */
#define OBSOLETE(name) { name, CONFIG_TYPE_OBSOLETE, 0, NULL }

/** Array of configuration options.  Until we disallow nonstandard
 * abbreviations, order is significant, since the first matching option will
 * be chosen first.
 */
static config_var_t _option_vars[] = {
  VAR("AccountingMax",       MEMUNIT,  AccountingMax,        "0 bytes"),
  VAR("AccountingMaxKB",     UINT,     _AccountingMaxKB,     "0"),
  VAR("AccountingStart",     STRING,   AccountingStart,      NULL),
  VAR("Address",             STRING,   Address,              NULL),
  VAR("AllowUnverifiedNodes",CSV,      AllowUnverifiedNodes,
                                                        "middle,rendezvous"),
  VAR("AssumeReachable",     BOOL,     AssumeReachable,      "0"),
  VAR("AuthDirInvalid",      LINELIST, AuthDirInvalid,       NULL),
  VAR("AuthDirReject",       LINELIST, AuthDirReject,        NULL),
  VAR("AuthDirRejectUnlisted",BOOL,    AuthDirRejectUnlisted,"0"),
  VAR("AuthoritativeDirectory",BOOL,   AuthoritativeDir,     "0"),
  VAR("BandwidthBurst",      MEMUNIT,  BandwidthBurst,       "5 MB"),
  VAR("BandwidthRate",       MEMUNIT,  BandwidthRate,        "2 MB"),
  VAR("ClientOnly",          BOOL,     ClientOnly,           "0"),
  VAR("ConnLimit",           UINT,     ConnLimit,            "1024"),
  VAR("ContactInfo",         STRING,   ContactInfo,          NULL),
  VAR("ControlPort",         UINT,     ControlPort,          "0"),
  VAR("CookieAuthentication",BOOL,     CookieAuthentication, "0"),
  VAR("DataDirectory",       STRING,   DataDirectory,        NULL),
  VAR("DebugLogFile",        STRING,   DebugLogFile,         NULL),
  VAR("DirAllowPrivateAddresses",BOOL, DirAllowPrivateAddresses, NULL),
  VAR("DirListenAddress",    LINELIST, DirListenAddress,     NULL),
  /* if DirFetchPeriod is 0, see get_dir_fetch_period() in main.c */
  VAR("DirFetchPeriod",      INTERVAL, DirFetchPeriod,       "0 seconds"),
  VAR("DirPolicy",           LINELIST, DirPolicy,            NULL),
  VAR("DirPort",             UINT,     DirPort,              "0"),
  OBSOLETE("DirPostPeriod"),
  VAR("DirServer",           LINELIST, DirServers,           NULL),
  VAR("EntryNodes",          STRING,   EntryNodes,           NULL),
  VAR("ExcludeNodes",        STRING,   ExcludeNodes,         NULL),
  VAR("ExitNodes",           STRING,   ExitNodes,            NULL),
  VAR("ExitPolicy",          LINELIST, ExitPolicy,           NULL),
  VAR("FascistFirewall",     BOOL,     FascistFirewall,      "0"),
  VAR("FirewallPorts",       CSV,      FirewallPorts,        ""),
  VAR("FastFirstHopPK",      BOOL,     FastFirstHopPK,       "1"),
  VAR("Group",               STRING,   Group,                NULL),
  VAR("HardwareAccel",       BOOL,     HardwareAccel,        "1"),
  VAR("HashedControlPassword",STRING,  HashedControlPassword, NULL),
  VAR("HiddenServiceDir",    LINELIST_S, RendConfigLines,    NULL),
  VAR("HiddenServiceExcludeNodes", LINELIST_S, RendConfigLines, NULL),
  VAR("HiddenServiceNodes",  LINELIST_S, RendConfigLines,    NULL),
  VAR("HiddenServiceOptions",LINELIST_V, RendConfigLines,    NULL),
  VAR("HiddenServicePort",   LINELIST_S, RendConfigLines,    NULL),
  VAR("HttpProxy",           STRING,   HttpProxy,            NULL),
  VAR("HttpProxyAuthenticator",STRING, HttpProxyAuthenticator,NULL),
  VAR("HttpsProxy",          STRING,   HttpsProxy,           NULL),
  VAR("HttpsProxyAuthenticator",STRING,HttpsProxyAuthenticator,NULL),
  OBSOLETE("IgnoreVersion"),
  VAR("KeepalivePeriod",     INTERVAL, KeepalivePeriod,      "5 minutes"),
  VAR("Log",                 LINELIST, Logs,                 NULL),
  OBSOLETE("LinkPadding"),
  VAR("LogFile",             LINELIST_S, OldLogOptions,      NULL),
  VAR("LogLevel",            LINELIST_S, OldLogOptions,      NULL),
  VAR("LongLivedPorts",      CSV,      LongLivedPorts,
                         "21,22,706,1863,5050,5190,5222,5223,6667,8300,8888"),
  VAR("MapAddress",          LINELIST, AddressMap,           NULL),
  VAR("MaxAdvertisedBandwidth",MEMUNIT,MaxAdvertisedBandwidth,"128 TB"),
  VAR("MaxCircuitDirtiness", INTERVAL, MaxCircuitDirtiness,  "10 minutes"),
  VAR("MaxOnionsPending",    UINT,     MaxOnionsPending,     "100"),
  OBSOLETE("MonthlyAccountingStart"),
  VAR("MyFamily",            STRING,   MyFamily,             NULL),
  VAR("NewCircuitPeriod",    INTERVAL, NewCircuitPeriod,     "30 seconds"),
  VAR("NamingAuthoritativeDirectory",BOOL, NamingAuthoritativeDir, "0"),
  VAR("Nickname",            STRING,   Nickname,             NULL),
  VAR("NoPublish",           BOOL,     NoPublish,            "0"),
  VAR("NodeFamily",          LINELIST, NodeFamilies,         NULL),
  VAR("NumCpus",             UINT,     NumCpus,              "1"),
  VAR("NumEntryGuards",      UINT,     NumEntryGuards,       "3"),
  VAR("ORListenAddress",     LINELIST, ORListenAddress,      NULL),
  VAR("ORPort",              UINT,     ORPort,               "0"),
  VAR("OutboundBindAddress", STRING,   OutboundBindAddress,  NULL),
  VAR("PathlenCoinWeight",   DOUBLE,   PathlenCoinWeight,    "0.3"),
  VAR("PidFile",             STRING,   PidFile,              NULL),
  VAR("ProtocolWarnings",    BOOL,     ProtocolWarnings,     "0"),
  VAR("ReachableAddresses",  LINELIST, ReachableAddresses,   NULL),
  VAR("RecommendedVersions", LINELIST, RecommendedVersions,  NULL),
  VAR("RecommendedClientVersions", LINELIST, RecommendedClientVersions,  NULL),
  VAR("RecommendedServerVersions", LINELIST, RecommendedServerVersions,  NULL),
  VAR("RedirectExit",        LINELIST, RedirectExit,         NULL),
  VAR("RendExcludeNodes",    STRING,   RendExcludeNodes,     NULL),
  VAR("RendNodes",           STRING,   RendNodes,            NULL),
  VAR("RendPostPeriod",      INTERVAL, RendPostPeriod,       "20 minutes"),
  VAR("RephistTrackTime",    INTERVAL, RephistTrackTime,     "24 hours"),
  OBSOLETE("RouterFile"),
  VAR("RunAsDaemon",         BOOL,     RunAsDaemon,          "0"),
  VAR("RunTesting",          BOOL,     RunTesting,           "0"),
  VAR("SafeLogging",         BOOL,     SafeLogging,          "1"),
  VAR("ShutdownWaitLength",  INTERVAL, ShutdownWaitLength,   "30 seconds"),
  VAR("SocksListenAddress",  LINELIST, SocksListenAddress,   NULL),
  VAR("SocksPolicy",         LINELIST, SocksPolicy,          NULL),
  VAR("SocksPort",           UINT,     SocksPort,            "9050"),
  /* if StatusFetchPeriod is 0, see get_status_fetch_period() in main.c */
  VAR("StatusFetchPeriod",   INTERVAL, StatusFetchPeriod,    "0 seconds"),
  VAR("StrictEntryNodes",    BOOL,     StrictEntryNodes,     "0"),
  VAR("StrictExitNodes",     BOOL,     StrictExitNodes,      "0"),
  VAR("SysLog",              LINELIST_S, OldLogOptions,      NULL),
  VAR("TestSocks",           BOOL,     TestSocks,            "0"),
  VAR("TrackHostExits",      CSV,      TrackHostExits,       NULL),
  VAR("TrackHostExitsExpire",INTERVAL, TrackHostExitsExpire, "30 minutes"),
  OBSOLETE("TrafficShaping"),
  VAR("UseEntryGuards",      BOOL,     UseEntryGuards,       "1"),
  VAR("User",                STRING,   User,                 NULL),
  VAR("V1AuthoritativeDirectory",BOOL, V1AuthoritativeDir,   "0"),
  VAR("VersioningAuthoritativeDirectory",BOOL,VersioningAuthoritativeDir, "0"),
  VAR("__LeaveStreamsUnattached", BOOL,LeaveStreamsUnattached, "0"),
  { NULL, CONFIG_TYPE_OBSOLETE, 0, NULL }
};
#undef VAR

#define VAR(name,conftype,member,initvalue)                             \
  { name, CONFIG_TYPE_ ## conftype, STRUCT_OFFSET(or_state_t, member),  \
      initvalue }
static config_var_t _state_vars[] = {
  VAR("AccountingBytesReadInInterval", MEMUNIT,
      AccountingBytesReadInInterval, NULL),
  VAR("AccountingBytesWrittenInInterval", MEMUNIT,
      AccountingBytesWrittenInInterval, NULL),
  VAR("AccountingExpectedUsage", MEMUNIT,     AccountingExpectedUsage, NULL),
  VAR("AccountingIntervalStart", ISOTIME,     AccountingIntervalStart, NULL),
  VAR("AccountingSecondsActive", INTERVAL,    AccountingSecondsActive, NULL),
  VAR("EntryGuard",              LINELIST_S,  EntryGuards,             NULL),
  VAR("EntryGuardDownSince",     LINELIST_S,  EntryGuards,             NULL),
  VAR("EntryGuardUnlistedSince", LINELIST_S,  EntryGuards,             NULL),
  VAR("EntryGuards",             LINELIST_V,  EntryGuards,             NULL),

  VAR("BWHistoryReadEnds",       ISOTIME,     BWHistoryReadEnds,      NULL),
  VAR("BWHistoryReadInterval",   UINT,        BWHistoryReadInterval,  NULL),
  VAR("BWHistoryReadValues",     CSV,         BWHistoryReadValues,    NULL),
  VAR("BWHistoryWriteEnds",      ISOTIME,     BWHistoryWriteEnds,     NULL),
  VAR("BWHistoryWriteInterval",  UINT,        BWHistoryWriteInterval, NULL),
  VAR("BWHistoryWriteValues",    CSV,         BWHistoryWriteValues,   NULL),

  VAR("TorVersion",              STRING,      TorVersion,             NULL),

  VAR("LastWritten",             ISOTIME,     LastWritten,            NULL),

  { NULL, CONFIG_TYPE_OBSOLETE, 0, NULL }
};

#undef VAR
#undef OBSOLETE

/** DOCDOC*/
typedef struct config_var_description_t {
  const char *name;
  const char *description;
} config_var_description_t;

static config_var_description_t options_description[] = {
  { "Address", "The advertised (external) address we should use." },
  // { "AccountingStart", ""},
  { NULL, NULL },
};

static config_var_description_t state_description[] = {
  { "AccountingBytesReadInInterval",
    "How many bytes have we read in this accounting period?" },
  { "AccountingBytesWrittenInInterval",
    "How many bytes have we written in this accounting period?" },
  { "AccountingExpectedUsage",
    "How many bytes did we expect to use per minute? (0 for no estimate.)" },
  { "AccountingIntervalStart", "When did this accounting period begin?" },
  { "AccountingSecondsActive", "How long have we been awake in this period?" },

  { "BWHistoryReadEnds", "When does the last-recorded read-interval end?" },
  { "BWHistoryReadInterval", "How long is each read-interval (in seconds)?" },
  { "BWHistoryReadValues", "Number of bytes read in each interval." },
  { "BWHistoryWriteEnds", "When does the last-recorded write-interval end?" },
  { "BWHistoryWriteInterval", "How long is each write-interval (in seconds)?"},
  { "BWHistoryWriteValues", "Number of bytes written in each interval." },

  { "EntryGuard", "One of the nodes we have chosen as a fixed entry" },
  { "EntryGuardDownSince",
    "The last entry guard has been down since this time." },
  { "EntryGuardUnlistedSince",
    "The last entry guard has been unlisted since this time." },
  { "LastWritten", "When was this state file last regenerated?" },

  { "TorVersion", "Which version of Tor generated this state file?" },
  { NULL, NULL },
};

typedef int (*validate_fn_t)(void*,void*,int);

/** Information on the keys, value types, key-to-struct-member mappings,
 * variable descriptions, validation functions, and abbreviations for a
 * configuration or storage format. */
typedef struct {
  size_t size;
  uint32_t magic;
  off_t magic_offset;
  config_abbrev_t *abbrevs;
  config_var_t *vars;
  validate_fn_t validate_fn;
  config_var_description_t *descriptions;
} config_format_t;

#define CHECK(fmt, cfg) do {                                            \
    tor_assert(fmt && cfg);                                             \
    tor_assert((fmt)->magic ==                                          \
               *(uint32_t*)(((char*)(cfg))+fmt->magic_offset));         \
  } while (0)

/** Largest allowed config line */
#define CONFIG_LINE_T_MAXLEN 4096

static void config_line_append(config_line_t **lst,
                               const char *key, const char *val);
static void option_clear(config_format_t *fmt, or_options_t *options,
                         config_var_t *var);
static void option_reset(config_format_t *fmt, or_options_t *options,
                         config_var_t *var, int use_defaults);
static void config_free(config_format_t *fmt, void *options);
static int option_is_same(config_format_t *fmt,
                          or_options_t *o1, or_options_t *o2,
                          const char *name);
static or_options_t *options_dup(config_format_t *fmt, or_options_t *old);
static int options_validate(or_options_t *old_options,
                            or_options_t *options, int from_setconf);
static int options_act_reversible(or_options_t *old_options);
static int options_act(or_options_t *old_options);
static int options_transition_allowed(or_options_t *old, or_options_t *new);
static int options_transition_affects_workers(or_options_t *old_options,
                                              or_options_t *new_options);
static int options_transition_affects_descriptor(or_options_t *old_options,
                                                 or_options_t *new_options);
static int check_nickname_list(const char *lst, const char *name);
static void config_register_addressmaps(or_options_t *options);

static int parse_dir_server_line(const char *line, int validate_only);
static int parse_redirect_line(smartlist_t *result,
                               config_line_t *line);
static int parse_log_severity_range(const char *range, int *min_out,
                                    int *max_out);
static int convert_log_option(or_options_t *options,
                              config_line_t *level_opt,
                              config_line_t *file_opt, int isDaemon);
static int add_single_log_option(or_options_t *options, int minSeverity,
                                 int maxSeverity,
                                 const char *type, const char *fname);
static int normalize_log_options(or_options_t *options);
static int validate_data_directory(or_options_t *options);
static int write_configuration_file(const char *fname, or_options_t *options);
static config_line_t *get_assigned_option(config_format_t *fmt,
                                     or_options_t *options, const char *key);
static void config_init(config_format_t *fmt, void *options);
static int or_state_validate(or_state_t *old_options, or_state_t *options,
                             int from_setconf);

static uint64_t config_parse_memunit(const char *s, int *ok);
static int config_parse_interval(const char *s, int *ok);
static void print_cvs_version(void);
static void parse_reachable_addresses(void);
static int init_libevent(void);
static int opt_streq(const char *s1, const char *s2);
#if defined(HAVE_EVENT_GET_VERSION) && defined(HAVE_EVENT_GET_METHOD)
static void check_libevent_version(const char *m, const char *v, int server);
#endif

/*static*/ or_options_t *options_new(void);

#define OR_OPTIONS_MAGIC 9090909

static config_format_t options_format = {
  sizeof(or_options_t),
  OR_OPTIONS_MAGIC,
  STRUCT_OFFSET(or_options_t, _magic),
  _option_abbrevs,
  _option_vars,
  (validate_fn_t)options_validate,
  options_description
};

#define OR_STATE_MAGIC 0x57A73f57

static config_format_t state_format = {
  sizeof(or_state_t),
  OR_STATE_MAGIC,
  STRUCT_OFFSET(or_state_t, _magic),
  _state_abbrevs,
  _state_vars,
  (validate_fn_t)or_state_validate,
  state_description
};

/*
 * Functions to read and write the global options pointer.
 */

/** Command-line and config-file options. */
static or_options_t *global_options = NULL;
/** Name of most recently read torrc file. */
static char *torrc_fname = NULL;
/** Persistent serialized state. */
static or_state_t *global_state = NULL;
/** DOCDOC */
static addr_policy_t *reachable_addr_policy = NULL;

static void *
config_alloc(config_format_t *fmt)
{
  void *opts = opts = tor_malloc_zero(fmt->size);
  *(uint32_t*)(((char*)opts)+fmt->magic_offset) = fmt->magic;
  CHECK(fmt, opts);
  return opts;
}

/** Return the currently configured options. */
or_options_t *
get_options(void)
{
  tor_assert(global_options);
  return global_options;
}

/** Change the current global options to contain <b>new_val</b> instead of
 * their current value; take action based on the new value; free the old value
 * as necessary.
 */
int
set_options(or_options_t *new_val)
{
  or_options_t *old_options = global_options;
  global_options = new_val;
  /* Note that we pass the *old* options below, for comparison. It
   * pulls the new options directly out of global_options. */
  if (options_act_reversible(old_options)<0) {
    global_options = old_options;
    return -1;
  }
  if (options_act(old_options) < 0) { /* acting on the options failed. die. */
    err(LD_CONFIG,
        "Acting on config options left us in a broken state. Dying.");
    exit(1);
  }
  if (old_options)
    config_free(&options_format, old_options);

  return 0;
}

void
config_free_all(void)
{
  if (global_options) {
    config_free(&options_format, global_options);
    global_options = NULL;
  }
  if (global_state) {
    config_free(&state_format, global_state);
    global_state = NULL;
  }
  tor_free(torrc_fname);
  if (reachable_addr_policy) {
    addr_policy_free(reachable_addr_policy);
    reachable_addr_policy = NULL;
  }
}

/** If options->SafeLogging is on, return a not very useful string,
 * else return address.
 */
const char *
safe_str(const char *address)
{
  if (get_options()->SafeLogging)
    return "[scrubbed]";
  else
    return address;
}

/** Add the default directory servers directly into the trusted dir list. */
static void
add_default_trusted_dirservers(void)
{
  const char *dirservers[] = {
"moria1 v1 18.244.0.188:9031 "
                           "FFCB 46DB 1339 DA84 674C 70D7 CB58 6434 C437 0441",
"moria2 v1 18.244.0.114:80  719B E45D E224 B607 C537 07D0 E214 3E2D 423E 74CF",
"tor26 v1 86.59.21.38:80    847B 1F85 0344 D787 6491 A548 92F9 0493 4E4E B85D"
  };
  parse_dir_server_line(dirservers[0], 0);
  parse_dir_server_line(dirservers[1], 0);
  parse_dir_server_line(dirservers[2], 0);
}

/** Fetch the active option list, and take actions based on it. All of the
 * things we do should survive being done repeatedly.  If present,
 * <b>old_options</b> contains the previous value of the options.
 *
 * Return 0 if all goes well, return -1 if things went badly.
 */
static int
options_act_reversible(or_options_t *old_options)
{
  smartlist_t *new_listeners = smartlist_create();
  smartlist_t *replaced_listeners = smartlist_create();
  static int libevent_initialized = 0;
  or_options_t *options = get_options();
  int running_tor = options->command == CMD_RUN_TOR;
  int set_conn_limit = 0;
  int r = -1;
  int logs_marked = 0;

  if (running_tor && options->RunAsDaemon) {
    /* No need to roll back, since you can't change the value. */
    start_daemon();
  }

  /* Setuid/setgid as appropriate */
  if (options->User || options->Group) {
    if (switch_id(options->User, options->Group) != 0) {
      /* No need to roll back, since you can't change the value. */
      goto done;
    }
  }

  /* Set up libevent. */
  if (running_tor && !libevent_initialized) {
    if (init_libevent())
      goto done;
    libevent_initialized = 1;
  }

  /* Ensure data directory is private; create if possible. */
  if (check_private_dir(options->DataDirectory, CPD_CREATE)<0) {
    err(LD_FS, "Couldn't access/create private data directory \"%s\"",
        options->DataDirectory);
    /* No need to roll back, since you can't change the value. */
    goto done;
  }

  /* Bail out at this point if we're not going to be a client or server:
   * we don't run  */
  if (options->command != CMD_RUN_TOR)
    goto commit;

  options->_ConnLimit =
    set_max_file_descriptors((unsigned)options->ConnLimit, MAXCONNECTIONS);
  if (options->_ConnLimit < 0)
    goto rollback;
  set_conn_limit = 1;

  if (retry_all_listeners(0, replaced_listeners, new_listeners) < 0) {
    err(LD_CONFIG, "Failed to bind one of the listener ports.");
    goto rollback;
  }

  mark_logs_temp(); /* Close current logs once new logs are open. */
  logs_marked = 1;
  if (options_init_logs(options, 0)<0) /* Configure the log(s) */
    goto rollback;

 commit:
  r = 0;
  if (logs_marked) {
    close_temp_logs();
    add_callback_log(LOG_ERR, LOG_ERR, control_event_logmsg);
    control_adjust_event_log_severity();
  }
  SMARTLIST_FOREACH(replaced_listeners, connection_t *, conn,
  {
    notice(LD_NET, "Closing old %s on %s:%d",
           conn_type_to_string(conn->type), conn->address, conn->port);
    connection_close_immediate(conn);
    connection_mark_for_close(conn);
  });
  goto done;

 rollback:
  r = -1;

  if (logs_marked) {
    rollback_log_changes();
    control_adjust_event_log_severity();
  }

  if (set_conn_limit && old_options)
    set_max_file_descriptors((unsigned)old_options->ConnLimit,MAXCONNECTIONS);

  SMARTLIST_FOREACH(new_listeners, connection_t *, conn,
  {
    notice(LD_NET, "Closing %s on %s:%d",
           conn_type_to_string(conn->type), conn->address, conn->port);
    connection_close_immediate(conn);
    connection_mark_for_close(conn);
  });

 done:
  smartlist_free(new_listeners);
  smartlist_free(replaced_listeners);
  return r;
}

/** Fetch the active option list, and take actions based on it. All of the
 * things we do should survive being done repeatedly.  If present,
 * <b>old_options</b> contains the previous value of the options.
 *
 * Return 0 if all goes well, return -1 if it's time to die.
 *
 * Note 2: We haven't moved all the "act on new configuration" logic
 * here yet.  Some is still in do_hup() and other places.
 */
static int
options_act(or_options_t *old_options)
{
  config_line_t *cl;
  char *fn;
  size_t len;
  or_options_t *options = get_options();
  int running_tor = options->command == CMD_RUN_TOR;

  clear_trusted_dir_servers();
  if (options->DirServers) {
    for (cl = options->DirServers; cl; cl = cl->next) {
      if (parse_dir_server_line(cl->value, 0)<0) {
        err(LD_BUG,
            "Bug: Previously validated DirServer line could not be added!");
        return -1;
      }
    }
  } else {
    add_default_trusted_dirservers();
  }

  if (running_tor && rend_config_services(options, 0)<0) {
    err(LD_BUG,
        "Bug: Previously validated hidden services line could not be added!");
    return -1;
  }

  if (running_tor) {
    len = strlen(options->DataDirectory)+32;
    fn = tor_malloc(len);
    tor_snprintf(fn, len, "%s/cached-status", options->DataDirectory);
    if (check_private_dir(fn, CPD_CREATE) != 0) {
      err(LD_CONFIG,
          "Couldn't access/create private data directory \"%s\"", fn);
      tor_free(fn);
      return -1;
    }
    tor_free(fn);
  }

  /* Bail out at this point if we're not going to be a client or server:
   * we want to not fork, and to log stuff to stderr. */
  if (options->command != CMD_RUN_TOR)
    return 0;

  /* Load state */
  if (! global_state)
    if (or_state_load())
      return -1;

  {
    smartlist_t *sl = smartlist_create();
    for (cl = options->RedirectExit; cl; cl = cl->next) {
      if (parse_redirect_line(sl, cl)<0)
        return -1;
    }
    set_exit_redirects(sl);
  }

  /* Finish backgrounding the process */
  if (running_tor && options->RunAsDaemon) {
    /* We may be calling this for the n'th time (on SIGHUP), but it's safe. */
    finish_daemon(options->DataDirectory);
  }

  /* Write our pid to the pid file. If we do not have write permissions we
   * will log a warning */
  if (running_tor && options->PidFile)
    write_pidfile(options->PidFile);

  /* Register addressmap directives */
  config_register_addressmaps(options);

  /* Update address policies. */
  parse_socks_policy();
  parse_dir_policy();
  parse_authdir_policy();
  parse_reachable_addresses();

  init_cookie_authentication(options->CookieAuthentication);

  /* reload keys as needed for rendezvous services. */
  if (rend_service_load_keys()<0) {
    err(LD_GENERAL,"Error loading rendezvous service keys");
    return -1;
  }

  /* Set up accounting */
  if (accounting_parse_options(options, 0)<0) {
    err(LD_CONFIG,"Error in accounting options");
    return -1;
  }
  if (accounting_is_enabled(options))
    configure_accounting(time(NULL));

  if (!running_tor)
    return 0;

  /* Check for transitions that need action. */
  if (old_options) {
    if (options->UseEntryGuards && !old_options->UseEntryGuards) {
      info(LD_CIRC,"Switching to entry guards; abandoning previous circuits");
      circuit_mark_all_unused_circs();
      circuit_expire_all_dirty_circs();
    }

    if (options_transition_affects_workers(old_options, options)) {
      info(LD_GENERAL,"Worker-related options changed. Rotating workers.");
      cpuworkers_rotate();
      dnsworkers_rotate();
    }
  }

  /* Check if we need to parse and add the EntryNodes config option. */
  if (options->EntryNodes &&
      (!old_options ||
       !opt_streq(old_options->EntryNodes, options->EntryNodes)))
    entry_nodes_should_be_added();

  /* Since our options changed, we might need to regenerate and upload our
   * server descriptor.
   */
  if (!old_options ||
      options_transition_affects_descriptor(old_options, options))
    mark_my_descriptor_dirty();

  return 0;
}

/*
 * Functions to parse config options
 */

/** If <b>option</b> is an official abbreviation for a longer option,
 * return the longer option.  Otherwise return <b>option</b>.
 * If <b>command_line</b> is set, apply all abbreviations.  Otherwise, only
 * apply abbreviations that work for the config file and the command line.
 * If <b>warn_obsolete</b> is set, warn about deprecated names. */
static const char *
expand_abbrev(config_format_t *fmt, const char *option, int command_line,
              int warn_obsolete)
{
  int i;
  if (! fmt->abbrevs)
    return option;
  for (i=0; fmt->abbrevs[i].abbreviated; ++i) {
    /* Abbreviations are casei. */
    if (!strcasecmp(option,fmt->abbrevs[i].abbreviated) &&
        (command_line || !fmt->abbrevs[i].commandline_only)) {
      if (warn_obsolete && fmt->abbrevs[i].warn) {
        warn(LD_CONFIG,
             "The configuration option '%s' is deprecated; use '%s' instead.",
             fmt->abbrevs[i].abbreviated,
             fmt->abbrevs[i].full);
      }
      return fmt->abbrevs[i].full;
    }
  }
  return option;
}

/** Helper: Read a list of configuration options from the command line.
 * If successful, put them in *<b>result</b> and return 0, and return
 * -1 and leave *<b>result</b> alone. */
static int
config_get_commandlines(int argc, char **argv, config_line_t **result)
{
  config_line_t *front = NULL;
  config_line_t **new = &front;
  char *s;
  int i = 1;

  while (i < argc) {
    if (!strcmp(argv[i],"-f") ||
        !strcmp(argv[i],"--hash-password")) {
      i += 2; /* command-line option with argument. ignore them. */
      continue;
    } else if (!strcmp(argv[i],"--list-fingerprint")) {
      i += 1; /* command-line option. ignore it. */
      continue;
    } else if (!strcmp(argv[i],"--nt-service")) {
      i += 1;
      continue;
    }
    if (i == argc-1) {
      warn(LD_CONFIG,"Command-line option '%s' with no value. Failing.",
           argv[i]);
      config_free_lines(front);
      return -1;
    }

    *new = tor_malloc_zero(sizeof(config_line_t));
    s = argv[i];

    while (*s == '-')
      s++;

    (*new)->key = tor_strdup(expand_abbrev(&options_format, s, 1, 1));
    (*new)->value = tor_strdup(argv[i+1]);
    (*new)->next = NULL;
    log(LOG_DEBUG, LD_CONFIG, "Commandline: parsed keyword '%s', value '%s'",
        (*new)->key, (*new)->value);

    new = &((*new)->next);
    i += 2;
  }
  *result = front;
  return 0;
}

/** Helper: allocate a new configuration option mapping 'key' to 'val',
 * append it to *<b>lst</b>. */
static void
config_line_append(config_line_t **lst,
                   const char *key,
                   const char *val)
{
  config_line_t *newline;

  newline = tor_malloc(sizeof(config_line_t));
  newline->key = tor_strdup(key);
  newline->value = tor_strdup(val);
  newline->next = NULL;
  while (*lst)
    lst = &((*lst)->next);

  (*lst) = newline;
}

/** Helper: parse the config string and strdup into key/value
 * strings. Set *result to the list, or NULL if parsing the string
 * failed.  Return 0 on success, -1 on failure. Warn and ignore any
 * misformatted lines. */
int
config_get_lines(char *string, config_line_t **result)
{
  config_line_t *list = NULL, **next;
  char *k, *v;

  next = &list;
  do {
    string = parse_line_from_str(string, &k, &v);
    if (!string) {
      config_free_lines(list);
      return -1;
    }
    if (k && v) {
      /* This list can get long, so we keep a pointer to the end of it
       * rather than using config_line_append over and over and getting n^2
       * performance.  This is the only really long list. */
      *next = tor_malloc(sizeof(config_line_t));
      (*next)->key = tor_strdup(k);
      (*next)->value = tor_strdup(v);
      (*next)->next = NULL;
      next = &((*next)->next);
    }
  } while (*string);

  *result = list;
  return 0;
}

/**
 * Free all the configuration lines on the linked list <b>front</b>.
 */
void
config_free_lines(config_line_t *front)
{
  config_line_t *tmp;

  while (front) {
    tmp = front;
    front = tmp->next;

    tor_free(tmp->key);
    tor_free(tmp->value);
    tor_free(tmp);
  }
}

/** DOCDOC */
static const char *
config_find_description(config_format_t *fmt, const char *name)
{
  int i;
  for (i=0; fmt->descriptions[i].name; ++i) {
    if (!strcasecmp(name, fmt->descriptions[i].name))
      return fmt->descriptions[i].description;
  }
  return NULL;
}

/** If <b>key</b> is a configuration option, return the corresponding
 * config_var_t.  Otherwise, if <b>key</b> is a non-standard abbreviation,
 * warn, and return the corresponding config_var_t.  Otherwise return NULL.
 */
static config_var_t *
config_find_option(config_format_t *fmt, const char *key)
{
  int i;
  size_t keylen = strlen(key);
  if (!keylen)
    return NULL; /* if they say "--" on the commandline, it's not an option */
  /* First, check for an exact (case-insensitive) match */
  for (i=0; fmt->vars[i].name; ++i) {
    if (!strcasecmp(key, fmt->vars[i].name)) {
      return &fmt->vars[i];
    }
  }
  /* If none, check for an abbreviated match */
  for (i=0; fmt->vars[i].name; ++i) {
    if (!strncasecmp(key, fmt->vars[i].name, keylen)) {
      warn(LD_CONFIG, "The abbreviation '%s' is deprecated. "
           "Please use '%s' instead",
           key, fmt->vars[i].name);
      return &fmt->vars[i];
    }
  }
  /* Okay, unrecognized options */
  return NULL;
}

/*
 * Functions to assign config options.
 */

/** <b>c</b>-\>key is known to be a real key. Update <b>options</b>
 * with <b>c</b>-\>value and return 0, or return -1 if bad value.
 *
 * Called from config_assign_line() and option_reset().
 */
static int
config_assign_value(config_format_t *fmt, or_options_t *options,
                    config_line_t *c)
{
  int i, ok;
  config_var_t *var;
  void *lvalue;

  CHECK(fmt, options);

  var = config_find_option(fmt, c->key);
  tor_assert(var);

  lvalue = ((char*)options) + var->var_offset;

  switch (var->type) {

  case CONFIG_TYPE_UINT:
    i = tor_parse_long(c->value, 10, 0, INT_MAX, &ok, NULL);
    if (!ok) {
      log(LOG_WARN, LD_CONFIG,
          "Int keyword '%s %s' is malformed or out of bounds.",
          c->key, c->value);
      return -1;
    }
    *(int *)lvalue = i;
    break;

  case CONFIG_TYPE_INTERVAL: {
    i = config_parse_interval(c->value, &ok);
    if (!ok) {
      return -1;
    }
    *(int *)lvalue = i;
    break;
  }

  case CONFIG_TYPE_MEMUNIT: {
    uint64_t u64 = config_parse_memunit(c->value, &ok);
    if (!ok) {
      return -1;
    }
    *(uint64_t *)lvalue = u64;
    break;
  }

  case CONFIG_TYPE_BOOL:
    i = tor_parse_long(c->value, 10, 0, 1, &ok, NULL);
    if (!ok) {
      log(LOG_WARN, LD_CONFIG, "Boolean keyword '%s' expects 0 or 1.", c->key);
      return -1;
    }
    *(int *)lvalue = i;
    break;

  case CONFIG_TYPE_STRING:
    tor_free(*(char **)lvalue);
    *(char **)lvalue = tor_strdup(c->value);
    break;

  case CONFIG_TYPE_DOUBLE:
    *(double *)lvalue = atof(c->value);
    break;

  case CONFIG_TYPE_ISOTIME:
    if (parse_iso_time(c->value, (time_t *)lvalue)) {
      log(LOG_WARN, LD_CONFIG,
          "Invalid time '%s' for keyword '%s'", c->value, c->key);
      return -1;
    }
    break;

  case CONFIG_TYPE_CSV:
    if (*(smartlist_t**)lvalue) {
      SMARTLIST_FOREACH(*(smartlist_t**)lvalue, char *, cp, tor_free(cp));
      smartlist_clear(*(smartlist_t**)lvalue);
    } else {
      *(smartlist_t**)lvalue = smartlist_create();
    }

    smartlist_split_string(*(smartlist_t**)lvalue, c->value, ",",
                           SPLIT_SKIP_SPACE|SPLIT_IGNORE_BLANK, 0);
    break;

  case CONFIG_TYPE_LINELIST:
  case CONFIG_TYPE_LINELIST_S:
    config_line_append((config_line_t**)lvalue, c->key, c->value);
    break;

  case CONFIG_TYPE_OBSOLETE:
    warn(LD_CONFIG, "Skipping obsolete configuration option '%s'", c->key);
    break;
  case CONFIG_TYPE_LINELIST_V:
    warn(LD_CONFIG, "Can't provide value for virtual option '%s'", c->key);
    return -1;
  default:
    tor_assert(0);
    break;
  }
  return 0;
}

/** If <b>c</b> is a syntactically valid configuration line, update
 * <b>options</b> with its value and return 0.  Otherwise return -1 for bad
 * key, -2 for bad value.
 *
 * If <b>clear_first</b> is set, clear the value first. Then if
 * <b>use_defaults</b> is set, set the value to the default.
 *
 * Called from config_assign().
 */
static int
config_assign_line(config_format_t *fmt, or_options_t *options,
                   config_line_t *c, int use_defaults, int clear_first)
{
  config_var_t *var;

  CHECK(fmt, options);

  var = config_find_option(fmt, c->key);
  if (!var) {
    warn(LD_CONFIG, "Unknown option '%s'.  Failing.", c->key);
    return -1;
  }
  /* Put keyword into canonical case. */
  if (strcmp(var->name, c->key)) {
    tor_free(c->key);
    c->key = tor_strdup(var->name);
  }

  if (!strlen(c->value)) { /* reset or clear it, then return */
    if (!clear_first) /* not already cleared */
      option_reset(fmt, options, var, use_defaults);
    return 0;
  }

  if (config_assign_value(fmt, options, c) < 0)
    return -2;
  return 0;
}

/** Restore the option named <b>key</b> in options to its default value.
 * Called from config_assign(). */
static void
config_reset_line(config_format_t *fmt, or_options_t *options,
                  const char *key, int use_defaults)
{
  config_var_t *var;

  CHECK(fmt, options);

  var = config_find_option(fmt, key);
  if (!var)
    return; /* give error on next pass. */

  option_reset(fmt, options, var, use_defaults);
}

/** Return true iff key is a valid configuration option. */
int
option_is_recognized(const char *key)
{
  config_var_t *var = config_find_option(&options_format, key);
  return (var != NULL);
}

/** Return the canonical name of a configuration option. */
const char *
option_get_canonical_name(const char *key)
{
  config_var_t *var = config_find_option(&options_format, key);
  return var->name;
}

/** Return a canonicalized list of the options assigned for key.
 */
config_line_t *
option_get_assignment(or_options_t *options, const char *key)
{
  return get_assigned_option(&options_format, options, key);
}

static config_line_t *
config_lines_dup(const config_line_t *inp)
{
  config_line_t *result = NULL;
  config_line_t **next_out = &result;
  while (inp) {
    *next_out = tor_malloc(sizeof(config_line_t));
    (*next_out)->key = tor_strdup(inp->key);
    (*next_out)->value = tor_strdup(inp->value);
    inp = inp->next;
    next_out = &((*next_out)->next);
  }
  (*next_out) = NULL;
  return result;
}

static config_line_t *
get_assigned_option(config_format_t *fmt, or_options_t *options,
                    const char *key)
{
  config_var_t *var;
  const void *value;
  char buf[32];
  config_line_t *result;
  tor_assert(options && key);

  CHECK(fmt, options);

  var = config_find_option(fmt, key);
  if (!var) {
    warn(LD_CONFIG, "Unknown option '%s'.  Failing.", key);
    return NULL;
  } else if (var->type == CONFIG_TYPE_LINELIST_S) {
    warn(LD_CONFIG,
         "Can't return context-sensitive '%s' on its own", key);
    return NULL;
  }
  value = ((char*)options) + var->var_offset;

  if (var->type == CONFIG_TYPE_LINELIST ||
      var->type == CONFIG_TYPE_LINELIST_V) {
    /* Linelist requires special handling: we just copy and return it. */
    return config_lines_dup(*(const config_line_t**)value);
  }

  result = tor_malloc_zero(sizeof(config_line_t));
  result->key = tor_strdup(var->name);
  switch (var->type)
    {
    case CONFIG_TYPE_STRING:
      if (*(char**)value) {
        result->value = tor_strdup(*(char**)value);
      } else {
        tor_free(result->key);
        tor_free(result);
        return NULL;
      }
      break;
    case CONFIG_TYPE_ISOTIME:
      if (*(time_t*)value) {
        result->value = tor_malloc(ISO_TIME_LEN+1);
        format_iso_time(result->value, *(time_t*)value);
      } else {
        tor_free(result->key);
        tor_free(result);
      }
      break;
    case CONFIG_TYPE_INTERVAL:
    case CONFIG_TYPE_UINT:
      /* This means every or_options_t uint or bool element
       * needs to be an int. Not, say, a uint16_t or char. */
      tor_snprintf(buf, sizeof(buf), "%d", *(int*)value);
      result->value = tor_strdup(buf);
      break;
    case CONFIG_TYPE_MEMUNIT:
      tor_snprintf(buf, sizeof(buf), U64_FORMAT,
                   U64_PRINTF_ARG(*(uint64_t*)value));
      result->value = tor_strdup(buf);
      break;
    case CONFIG_TYPE_DOUBLE:
      tor_snprintf(buf, sizeof(buf), "%f", *(double*)value);
      result->value = tor_strdup(buf);
      break;
    case CONFIG_TYPE_BOOL:
      result->value = tor_strdup(*(int*)value ? "1" : "0");
      break;
    case CONFIG_TYPE_CSV:
      if (*(smartlist_t**)value)
        result->value =
          smartlist_join_strings(*(smartlist_t**)value, ",", 0, NULL);
      else
        result->value = tor_strdup("");
      break;
    case CONFIG_TYPE_OBSOLETE:
      warn(LD_CONFIG,
           "You asked me for the value of an obsolete config option '%s'.",
           key);
      tor_free(result->key);
      tor_free(result);
      return NULL;
    default:
      tor_free(result->key);
      tor_free(result);
      warn(LD_BUG,"Bug: unknown type %d for known key '%s'", var->type, key);
      return NULL;
    }

  return result;
}

/** Iterate through the linked list of requested options <b>list</b>.
 * For each item, convert as appropriate and assign to <b>options</b>.
 * If an item is unrecognized, return -1 immediately,
 * else return 0 for success.
 *
 * If <b>clear_first</b>, interpret config options as replacing (not
 * extending) their previous values. If <b>clear_first</b> is set,
 * then <b>use_defaults</b> to decide if you set to defaults after
 * clearing, or make the value 0 or NULL.
 *
 * Here are the use cases:
 * 1. A non-empty AllowUnverified line in your torrc. Appends to current
 *    if linelist, replaces current if csv.
 * 2. An empty AllowUnverified line in your torrc. Should clear it.
 * 3. "RESETCONF AllowUnverified" sets it to default.
 * 4. "SETCONF AllowUnverified" makes it NULL.
 * 5. "SETCONF AllowUnverified=foo" clears it and sets it to "foo".
 *
 * Use_defaults   Clear_first
 *    0                0       "append"
 *    1                0       undefined, don't use
 *    0                1       "set to null first"
 *    1                1       "set to defaults first"
 * Return 0 on success, -1 on bad key, -2 on bad value.
 */

/*
There are three call cases for config_assign() currently.

Case one: Torrc entry
options_init_from_torrc() calls config_assign(0, 0)
  calls config_assign_line(0, 0).
    if value is empty, calls option_reset(0) and returns.
    calls config_assign_value(), appends.

Case two: setconf
options_trial_assign() calls config_assign(0, 1)
  calls config_reset_line(0)
    calls option_reset(0)
      calls option_clear().
  calls config_assign_line(0, 1).
    if value is empty, returns.
    calls config_assign_value(), appends.

Case three: resetconf
options_trial_assign() calls config_assign(1, 1)
  calls config_reset_line(1)
    calls option_reset(1)
      calls option_clear().
      calls config_assign_value(default)
  calls config_assign_line(1, 1).
    returns.
*/
static int
config_assign(config_format_t *fmt, void *options,
              config_line_t *list, int use_defaults, int clear_first)
{
  config_line_t *p;

  CHECK(fmt, options);

  /* pass 1: normalize keys */
  for (p = list; p; p = p->next) {
    const char *full = expand_abbrev(fmt, p->key, 0, 1);
    if (strcmp(full,p->key)) {
      tor_free(p->key);
      p->key = tor_strdup(full);
    }
  }

  /* pass 2: if we're reading from a resetting source, clear all
   * mentioned config options, and maybe set to their defaults. */
  if (clear_first) {
    for (p = list; p; p = p->next)
      config_reset_line(fmt, options, p->key, use_defaults);
  }

  /* pass 3: assign. */
  while (list) {
    int r;
    if ((r=config_assign_line(fmt, options, list, use_defaults, clear_first)))
      return r;
    list = list->next;
  }
  return 0;
}

/** Try assigning <b>list</b> to the global options. You do this by duping
 * options, assigning list to the new one, then validating it. If it's
 * ok, then throw out the old one and stick with the new one. Else,
 * revert to old and return failure.  Return 0 on success, -1 on bad
 * keys, -2 on bad values, -3 on bad transition, and -4 on failed-to-set.
 */
int
options_trial_assign(config_line_t *list, int use_defaults, int clear_first)
{
  int r;
  or_options_t *trial_options = options_dup(&options_format, get_options());

  if ((r=config_assign(&options_format, trial_options,
                       list, use_defaults, clear_first)) < 0) {
    config_free(&options_format, trial_options);
    return r;
  }

  if (options_validate(get_options(), trial_options, 1) < 0) {
    config_free(&options_format, trial_options);
    return -2;
  }

  if (options_transition_allowed(get_options(), trial_options) < 0) {
    config_free(&options_format, trial_options);
    return -3;
  }

  if (set_options(trial_options)<0) {
    config_free(&options_format, trial_options);
    return -4;
  }

  /* we liked it. put it in place. */
  return 0;
}

/** Reset config option <b>var</b> to 0, 0.0, NULL, or the equivalent.
 * Called from option_reset() and config_free(). */
static void
option_clear(config_format_t *fmt, or_options_t *options, config_var_t *var)
{
  void *lvalue = ((char*)options) + var->var_offset;
  switch (var->type) {
    case CONFIG_TYPE_STRING:
      tor_free(*(char**)lvalue);
      break;
    case CONFIG_TYPE_DOUBLE:
      *(double*)lvalue = 0.0;
      break;
    case CONFIG_TYPE_ISOTIME:
      *(time_t*)lvalue = 0;
    case CONFIG_TYPE_INTERVAL:
    case CONFIG_TYPE_UINT:
    case CONFIG_TYPE_BOOL:
      *(int*)lvalue = 0;
      break;
    case CONFIG_TYPE_MEMUNIT:
      *(uint64_t*)lvalue = 0;
      break;
    case CONFIG_TYPE_CSV:
      if (*(smartlist_t**)lvalue) {
        SMARTLIST_FOREACH(*(smartlist_t **)lvalue, char *, cp, tor_free(cp));
        smartlist_free(*(smartlist_t **)lvalue);
        *(smartlist_t **)lvalue = NULL;
      }
      break;
    case CONFIG_TYPE_LINELIST:
    case CONFIG_TYPE_LINELIST_S:
      config_free_lines(*(config_line_t **)lvalue);
      *(config_line_t **)lvalue = NULL;
      break;
    case CONFIG_TYPE_LINELIST_V:
      /* handled by linelist_s. */
      break;
    case CONFIG_TYPE_OBSOLETE:
      break;
  }
}

/** Clear the option indexed by <b>var</b> in <b>options</b>. Then if
 * <b>use_defaults</b>, set it to its default value.
 * Called by config_init() and option_reset_line() and option_assign_line(). */
static void
option_reset(config_format_t *fmt, or_options_t *options,
             config_var_t *var, int use_defaults)
{
  config_line_t *c;
  void *lvalue;
  CHECK(fmt, options);
  option_clear(fmt, options, var); /* clear it first */
  if (!use_defaults)
    return; /* all done */
  lvalue = ((char*)options) + var->var_offset;
  if (var->initvalue) {
    c = tor_malloc_zero(sizeof(config_line_t));
    c->key = tor_strdup(var->name);
    c->value = tor_strdup(var->initvalue);
    config_assign_value(fmt, options, c);
    config_free_lines(c);
  }
}

/** Print a usage message for tor. */
static void
print_usage(void)
{
  printf(
"Copyright 2001-2005 Roger Dingledine, Nick Mathewson.\n\n"
"tor -f <torrc> [args]\n"
"See man page for options, or http://tor.eff.org/ for documentation.\n");
}

/**
 * Based on <b>options-\>Address</b>, guess our public IP address and put it
 * (in host order) into *<b>addr_out</b>. If <b>hostname_out</b> is provided,
 * set *<b>hostname_out</b> to a new string holding the hostname we used to
 * get the address. Return 0 if all is well, or -1 if we can't find a suitable
 * public IP address.
 */
int
resolve_my_address(or_options_t *options, uint32_t *addr_out,
                   char **hostname_out)
{
  struct in_addr in;
  struct hostent *rent;
  char hostname[256];
  int explicit_ip=1;
  char tmpbuf[INET_NTOA_BUF_LEN];
  static uint32_t old_addr=0;
  const char *address = options->Address;

  tor_assert(addr_out);

  if (address && *address) {
    strlcpy(hostname, address, sizeof(hostname));
  } else { /* then we need to guess our address */
    explicit_ip = 0; /* it's implicit */

    if (gethostname(hostname, sizeof(hostname)) < 0) {
      warn(LD_NET,"Error obtaining local hostname");
      return -1;
    }
    debug(LD_CONFIG,"Guessed local host name as '%s'",hostname);
  }

  /* now we know hostname. resolve it and keep only the IP */

  if (tor_inet_aton(hostname, &in) == 0) {
    /* then we have to resolve it */
    explicit_ip = 0;
    rent = (struct hostent *)gethostbyname(hostname);
    if (!rent) {
      warn(LD_CONFIG,"Could not resolve local Address '%s'. Failing.",
           hostname);
      return -1;
    }
    tor_assert(rent->h_length == 4);
    memcpy(&in.s_addr, rent->h_addr, rent->h_length);
  }

  tor_inet_ntoa(&in,tmpbuf,sizeof(tmpbuf));
  if (is_internal_IP(htonl(in.s_addr), 0) && !options->NoPublish) {
    /* make sure we're ok with publishing an internal IP */
    if (!options->DirServers) {
      /* if they are using the default dirservers, disallow internal IPs
       * always. */
      warn(LD_CONFIG,"Address '%s' resolves to private IP '%s'. "
           "Tor servers that use the default DirServers must have public "
           "IP addresses.",
           hostname, tmpbuf);
      return -1;
    }
    if (!explicit_ip) {
      /* even if they've set their own dirservers, require an explicit IP if
       * they're using an internal address. */
      warn(LD_CONFIG,"Address '%s' resolves to private IP '%s'. Please "
           "set the Address config option to be the IP you want to use.",
           hostname, tmpbuf);
      return -1;
    }
  }

  debug(LD_CONFIG, "Resolved Address to '%s'.", tmpbuf);
  *addr_out = ntohl(in.s_addr);
  if (old_addr && old_addr != *addr_out) {
    notice(LD_NET, "Your IP seems to have changed. Updating.");
    server_has_changed_ip();
  }
  old_addr = *addr_out;
  if (hostname_out)
    *hostname_out = tor_strdup(hostname);
  return 0;
}

/** Called when we don't have a nickname set.  Try to guess a good nickname
 * based on the hostname, and return it in a newly allocated string. */
static char *
get_default_nickname(void)
{
  char localhostname[256];
  char *cp, *out, *outp;

  if (gethostname(localhostname, sizeof(localhostname)) < 0) {
    warn(LD_NET,"Error obtaining local hostname");
    return NULL;
  }

  /* Put it in lowercase; stop at the first dot. */
  for (cp = localhostname; *cp; ++cp) {
    if (*cp == '.') {
      *cp = '\0';
      break;
    }
    *cp = tolower(*cp);
  }

  /* Strip invalid characters. */
  cp = localhostname;
  out = outp = tor_malloc(strlen(localhostname) + 1);
  while (*cp) {
    if (strchr(LEGAL_NICKNAME_CHARACTERS, *cp))
      *outp++ = *cp++;
    else
      cp++;
  }
  *outp = '\0';

  /* Enforce length. */
  if (strlen(out) > MAX_NICKNAME_LEN)
    out[MAX_NICKNAME_LEN]='\0';

  return out;
}

/** Release storage held by <b>options</b> */
static void
config_free(config_format_t *fmt, void *options)
{
  int i;

  tor_assert(options);

  for (i=0; fmt->vars[i].name; ++i)
    option_clear(fmt, options, &(fmt->vars[i]));
  tor_free(options);
}

/** Return true iff a and b contain identical keys and values in identical
 * order. */
static int
config_lines_eq(config_line_t *a, config_line_t *b)
{
  while (a && b) {
    if (strcasecmp(a->key, b->key) || strcmp(a->value, b->value))
      return 0;
    a = a->next;
    b = b->next;
  }
  if (a || b)
    return 0;
  return 1;
}

/** Return true iff the option <b>var</b> has the same value in <b>o1</b>
 * and <b>o2</b>.  Must not be called for LINELIST_S or OBSOLETE options.
 */
static int
option_is_same(config_format_t *fmt,
               or_options_t *o1, or_options_t *o2, const char *name)
{
  config_line_t *c1, *c2;
  int r = 1;
  CHECK(fmt, o1);
  CHECK(fmt, o2);

  c1 = get_assigned_option(fmt, o1, name);
  c2 = get_assigned_option(fmt, o2, name);
  r = config_lines_eq(c1, c2);
  config_free_lines(c1);
  config_free_lines(c2);
  return r;
}

/** Copy storage held by <b>old</b> into a new or_options_t and return it. */
static or_options_t *
options_dup(config_format_t *fmt, or_options_t *old)
{
  or_options_t *newopts;
  int i;
  config_line_t *line;

  newopts = config_alloc(fmt);
  for (i=0; fmt->vars[i].name; ++i) {
    if (fmt->vars[i].type == CONFIG_TYPE_LINELIST_S)
      continue;
    if (fmt->vars[i].type == CONFIG_TYPE_OBSOLETE)
      continue;
    line = get_assigned_option(fmt, old, fmt->vars[i].name);
    if (line) {
      if (config_assign(fmt, newopts, line, 0, 0) < 0) {
        warn(LD_BUG, "Bug: config_get_assigned_option() generated "
             "something we couldn't config_assign().");
        tor_assert(0);
      }
    }
    config_free_lines(line);
  }
  return newopts;
}

/** Return a new empty or_options_t.  Used for testing. */
or_options_t *
options_new(void)
{
  return config_alloc(&options_format);
}

/** Set <b>options</b> to hold reasonable defaults for most options.
 * Each option defaults to zero. */
void
options_init(or_options_t *options)
{
  config_init(&options_format, options);
}

/* DOCDOC */
static void
config_init(config_format_t *fmt, void *options)
{
  int i;
  config_var_t *var;
  CHECK(fmt, options);

  for (i=0; fmt->vars[i].name; ++i) {
    var = &fmt->vars[i];
    if (!var->initvalue)
      continue; /* defaults to NULL or 0 */
    option_reset(fmt, options, var, 1);
  }
}

/* DOCDOC */
static char *
config_dump(config_format_t *fmt, void *options, int minimal)
{
  smartlist_t *elements;
  or_options_t *defaults;
  config_line_t *line, *assigned;
  char *result;
  int i;
  const char *desc;

  defaults = config_alloc(fmt);
  config_init(fmt, defaults);
  fmt->validate_fn(NULL,defaults, 1);
    /* XXX use a 1 here so we don't add a new log line while dumping */

  elements = smartlist_create();
  for (i=0; fmt->vars[i].name; ++i) {
    if (fmt->vars[i].type == CONFIG_TYPE_OBSOLETE ||
        fmt->vars[i].type == CONFIG_TYPE_LINELIST_S)
      continue;
    /* Don't save 'hidden' control variables. */
    if (!strcmpstart(fmt->vars[i].name, "__"))
      continue;
    if (minimal && option_is_same(fmt, options, defaults, fmt->vars[i].name))
      continue;

    desc = config_find_description(fmt, fmt->vars[i].name);
    line = assigned = get_assigned_option(fmt, options, fmt->vars[i].name);

    if (line && desc) {
      /* Only dump the description if there's something to describe. */
      size_t len = strlen(desc)+8;
      char *tmp = tor_malloc(len);
      tor_snprintf(tmp, len, "# %s\n",desc);
      smartlist_add(elements, tmp);
    }

    for (; line; line = line->next) {
      size_t len = strlen(line->key) + strlen(line->value) + 3;
      char *tmp;
      tmp = tor_malloc(len);
      if (tor_snprintf(tmp, len, "%s %s\n", line->key, line->value)<0) {
        err(LD_BUG,"Internal error writing log option");
        tor_assert(0);
      }
      smartlist_add(elements, tmp);
    }
    config_free_lines(assigned);
  }

  result = smartlist_join_strings(elements, "", 0, NULL);
  SMARTLIST_FOREACH(elements, char *, cp, tor_free(cp));
  smartlist_free(elements);
  config_free(fmt, defaults);
  return result;
}

/** Return a string containing a possible configuration file that would give
 * the configuration in <b>options</b>.  If <b>minimal</b> is true, do not
 * include options that are the same as Tor's defaults.
 */
char *
options_dump(or_options_t *options, int minimal)
{
  return config_dump(&options_format, options, minimal);
}

/* Return 0 if every element of sl is a string holding a decimal
 * representation of a port number, or if sl is NULL.
 * Otherwise return -1. */
static int
validate_ports_csv(smartlist_t *sl, const char *name)
{
  int i;
  int result = 0;
  tor_assert(name);

  if (!sl)
    return 0;

  SMARTLIST_FOREACH(sl, const char *, cp,
  {
    i = atoi(cp);
    if (i < 1 || i > 65535) {
      log(LOG_WARN, LD_CONFIG, "Port '%s' out of range in %s", cp, name);
      result=-1;
    }
  });
  return result;
}

/** DOCDOC */
static void
parse_reachable_addresses(void)
{
  or_options_t *options = get_options();

  addr_policy_free(reachable_addr_policy);
  reachable_addr_policy = NULL;

  if (config_parse_addr_policy(options->ReachableAddresses,
                               &reachable_addr_policy,
                               ADDR_POLICY_ACCEPT)) {
    warn(LD_CONFIG, "Error in ReachableAddresses entry; ignoring.");
    return;
  }
}

/** Return true iff the firewall options might block any address:port
 * combination.
 */
int
firewall_is_fascist(void)
{
  return reachable_addr_policy ? 1 : 0;
}

/** Return true iff we are configured to think that the local fascist
 * firewall (if any) will allow a connection to <b>addr</b>:<b>port</b>. */
int
fascist_firewall_allows_address(uint32_t addr, uint16_t port)
{
  addr_policy_result_t p = router_compare_addr_to_addr_policy(
               addr, port, reachable_addr_policy);

  switch (p) {
    case ADDR_POLICY_PROBABLY_ACCEPTED:
    case ADDR_POLICY_ACCEPTED:
      return 1;
    case ADDR_POLICY_PROBABLY_REJECTED:
    case ADDR_POLICY_REJECTED:
      return 0;
    default:
      warn(LD_BUG, "Unexpected result: %d", (int)p);
      return 0;
  }
}

/** Return 0 if every setting in <b>options</b> is reasonable.  Else
 * warn and return -1.  Should have no side effects, except for
 * normalizing the contents of <b>options</b>.
 * DOCDOC old_options.
 *
 * XXX
 * If <b>from_setconf</b>, we were called by the controller, and our
 * Log line should stay empty. If it's 0, then give us a default log
 * if there are no logs defined.
 */
static int
options_validate(or_options_t *old_options, or_options_t *options,
                 int from_setconf)
{
  int result = 0;
  config_line_t *cl;
  addr_policy_t *addr_policy=NULL;
#define REJECT(arg) \
  do { log(LOG_WARN, LD_CONFIG, arg); result = -1; } while (0)
#define COMPLAIN(arg) do { log(LOG_WARN, LD_CONFIG, arg); } while (0)

  if (options->ORPort < 0 || options->ORPort > 65535)
    REJECT("ORPort option out of bounds.");

  if (options->ORPort == 0 && options->ORListenAddress != NULL)
    REJECT("ORPort must be defined if ORListenAddress is defined.");

  if (options->DirPort == 0 && options->DirListenAddress != NULL)
    REJECT("DirPort must be defined if DirListenAddress is defined.");

#if 0 /* don't complain, since a standard configuration does this! */
  if (options->SocksPort == 0 && options->SocksListenAddress != NULL)
    REJECT("SocksPort must be defined if SocksListenAddress is defined.");
#endif

  if (options->SocksListenAddress) {
    config_line_t *line = NULL;
    char *address = NULL;
    for (line = options->SocksListenAddress; line; line = line->next) {
      uint16_t port;
      uint32_t addr;
      if (parse_addr_port(line->value, &address, &addr, &port)<0)
        continue; /* We'll warn about this later. */
      if (!is_internal_IP(addr, 1) &&
          (!old_options || !config_lines_eq(old_options->SocksListenAddress,
                                            options->SocksListenAddress))) {
        warn(LD_CONFIG,
             "You specified a public address '%s' for a SOCKS listener. Other "
             "people on the Internet might find your computer and use it as "
             "an open SOCKS proxy. Please don't allow this unless you have "
             "a good reason.", address);
      }
      tor_free(address);
    }
  }

  if (validate_data_directory(options)<0)
    REJECT("Invalid DataDirectory");

  if (options->Nickname == NULL) {
    if (server_mode(options)) {
      if (!(options->Nickname = get_default_nickname()))
        return -1;
      notice(LD_CONFIG, "Choosing default nickname '%s'", options->Nickname);
    }
  } else {
    if (!is_legal_nickname(options->Nickname)) {
      log(LOG_WARN, LD_CONFIG,
          "Nickname '%s' is wrong length or contains illegal characters.",
          options->Nickname);
      result = -1;
    }
  }

  if (server_mode(options) && !options->ContactInfo)
    log(LOG_NOTICE, LD_CONFIG, "Your ContactInfo config option is not set. "
        "Please consider setting it, so we can contact you if your server is "
        "misconfigured or something else goes wrong.");

  if (normalize_log_options(options))
    return -1;

  /* Special case on first boot if no Log options are given. */
  if (!options->Logs && !from_setconf) {
    config_line_append(&options->Logs, "Log", "notice stdout");
  }

  if (options_init_logs(options, 1)<0) /* Validate the log(s) */
    return -1;

  if (server_mode(options)) {
    /* confirm that our address isn't broken, so we can complain now */
    uint32_t tmp;
    if (resolve_my_address(options, &tmp, NULL) < 0)
      result = -1;
  }

#ifndef MS_WINDOWS
  if (options->RunAsDaemon && torrc_fname && path_is_relative(torrc_fname))
    REJECT("Can't use a relative path to torrc when RunAsDaemon is set.");
#endif

  if (options->SocksPort < 0 || options->SocksPort > 65535)
    REJECT("SocksPort option out of bounds.");

  if (options->SocksPort == 0 && options->ORPort == 0)
    REJECT("SocksPort and ORPort are both undefined? Quitting.");

  if (options->ControlPort < 0 || options->ControlPort > 65535)
    REJECT("ControlPort option out of bounds.");

  if (options->DirPort < 0 || options->DirPort > 65535)
    REJECT("DirPort option out of bounds.");

  if (options->StrictExitNodes &&
      (!options->ExitNodes || !strlen(options->ExitNodes)) &&
      (!old_options ||
       (old_options->StrictExitNodes != options->StrictExitNodes) ||
       (!opt_streq(old_options->ExitNodes, options->ExitNodes))))
    COMPLAIN("StrictExitNodes set, but no ExitNodes listed.");

  if (options->StrictEntryNodes &&
      (!options->EntryNodes || !strlen(options->EntryNodes)) &&
      (!old_options ||
       (old_options->StrictEntryNodes != options->StrictEntryNodes) ||
       (!opt_streq(old_options->EntryNodes, options->EntryNodes))))
    COMPLAIN("StrictEntryNodes set, but no EntryNodes listed.");

  if (options->AuthoritativeDir) {
    if (!options->ContactInfo)
      REJECT("Authoritative directory servers must set ContactInfo");
    if (!options->RecommendedVersions)
      REJECT("Authoritative directory servers must set RecommendedVersions.");
    if (!options->RecommendedClientVersions)
      options->RecommendedClientVersions =
        config_lines_dup(options->RecommendedVersions);
    if (!options->RecommendedServerVersions)
      options->RecommendedServerVersions =
        config_lines_dup(options->RecommendedVersions);
    if (options->UseEntryGuards) {
      notice(LD_CONFIG, "Authoritative directory servers can't set "
             "UseEntryGuards. Disabling.");
      options->UseEntryGuards = 0;
    }
  }

  if (options->AuthoritativeDir && !options->DirPort)
    REJECT("Running as authoritative directory, but no DirPort set.");

  if (options->AuthoritativeDir && !options->ORPort)
    REJECT("Running as authoritative directory, but no ORPort set.");

  if (options->AuthoritativeDir && options->ClientOnly)
    REJECT("Running as authoritative directory, but ClientOnly also set.");

  if (options->AuthoritativeDir && options->NoPublish)
    REJECT("You cannot set both AuthoritativeDir and NoPublish.");

  if (options->ConnLimit <= 0) {
    log(LOG_WARN, LD_CONFIG,
        "ConnLimit must be greater than 0, but was set to %d",
        options->ConnLimit);
    result = -1;
  }

  if (options->_AccountingMaxKB) {
    log(LOG_WARN, LD_CONFIG, "AccountingMaxKB is deprecated.  "
        "Say 'AccountingMax %d KB' instead.", options->_AccountingMaxKB);
    options->AccountingMax = U64_LITERAL(1024)*options->_AccountingMaxKB;
    options->_AccountingMaxKB = 0;
  }

  if (validate_ports_csv(options->FirewallPorts, "FirewallPorts") < 0)
    result = -1;

  if (validate_ports_csv(options->LongLivedPorts, "LongLivedPorts") < 0)
    result = -1;

  if (options->FascistFirewall && !options->ReachableAddresses) {
    smartlist_t *instead = smartlist_create();
    config_line_t *new_line = tor_malloc_zero(sizeof(config_line_t));
    new_line->key = tor_strdup("ReachableAddresses");
    /* If we're configured with the old format, we need to prepend some
     * open ports. */
    if (!smartlist_len(options->FirewallPorts)) {
      smartlist_add(options->FirewallPorts, tor_strdup("80"));
      smartlist_add(options->FirewallPorts, tor_strdup("443"));
    }
    SMARTLIST_FOREACH(options->FirewallPorts, const char *, portno,
      {
        int p = atoi(portno);
        char *s;
        if (p<0) continue;
        s = tor_malloc(16);
        tor_snprintf(s, 16, "*:%d", p);
        smartlist_add(instead, s);
      });
    new_line->value = smartlist_join_strings(instead,",",0,NULL);
    /* These have been deprecated since 0.1.1.5-alpha-cvs */
    log(LOG_NOTICE, LD_CONFIG, "Converting FascistFirewall and FirewallPorts "
        "config options to new format: \"ReachableAddresses %s\"",
        new_line->value);
    options->ReachableAddresses = new_line;
    SMARTLIST_FOREACH(instead, char *, cp, tor_free(cp));
    smartlist_free(instead);
  }

  if (options->ReachableAddresses) {
    /* We need to end with a reject *:*, not an implicit accept *:* */
    config_line_t **linep = &options->ReachableAddresses;
    for (;;) {
      if (!strcmp((*linep)->value, "reject *:*")) /* already there */
        break;
      linep = &((*linep)->next);
      if (!*linep) {
        *linep = tor_malloc_zero(sizeof(config_line_t));
        (*linep)->key = tor_strdup("ReachableAddresses");
        (*linep)->value = tor_strdup("reject *:*");
        break;
      }
    }
  }

  if (options->ReachableAddresses && server_mode(options))
    REJECT("Servers must be able to freely connect to the rest "
           "of the Internet, so they must not set ReachableAddresses "
           "or FascistFirewall.");

  options->_AllowUnverified = 0;
  if (options->AllowUnverifiedNodes) {
    SMARTLIST_FOREACH(options->AllowUnverifiedNodes, const char *, cp, {
        if (!strcasecmp(cp, "entry"))
          options->_AllowUnverified |= ALLOW_UNVERIFIED_ENTRY;
        else if (!strcasecmp(cp, "exit"))
          options->_AllowUnverified |= ALLOW_UNVERIFIED_EXIT;
        else if (!strcasecmp(cp, "middle"))
          options->_AllowUnverified |= ALLOW_UNVERIFIED_MIDDLE;
        else if (!strcasecmp(cp, "introduction"))
          options->_AllowUnverified |= ALLOW_UNVERIFIED_INTRODUCTION;
        else if (!strcasecmp(cp, "rendezvous"))
          options->_AllowUnverified |= ALLOW_UNVERIFIED_RENDEZVOUS;
        else {
          log(LOG_WARN, LD_CONFIG,
              "Unrecognized value '%s' in AllowUnverifiedNodes", cp);
          result = -1;
        }
      });
  }

  if (options->SocksPort >= 1 &&
      (options->PathlenCoinWeight < 0.0 || options->PathlenCoinWeight >= 1.0))
    REJECT("PathlenCoinWeight option must be >=0.0 and <1.0.");

#define MIN_DIR_FETCH_PERIOD 600
#define MIN_REND_POST_PERIOD 300
#define MIN_STATUS_FETCH_PERIOD 60

#define MAX_DIR_PERIOD (MIN_ONION_KEY_LIFETIME/2)
#define MAX_CACHE_DIR_FETCH_PERIOD 3600
#define MAX_CACHE_STATUS_FETCH_PERIOD 900

  if (options->DirFetchPeriod &&
      options->DirFetchPeriod < MIN_DIR_FETCH_PERIOD) {
    log(LOG_WARN, LD_CONFIG,
        "DirFetchPeriod option must be at least %d seconds. Clipping.",
        MIN_DIR_FETCH_PERIOD);
    options->DirFetchPeriod = MIN_DIR_FETCH_PERIOD;
  }
  if (options->StatusFetchPeriod &&
      options->StatusFetchPeriod < MIN_STATUS_FETCH_PERIOD) {
    log(LOG_WARN, LD_CONFIG,
        "StatusFetchPeriod option must be at least %d seconds. Clipping.",
        MIN_STATUS_FETCH_PERIOD);
    options->StatusFetchPeriod = MIN_STATUS_FETCH_PERIOD;
  }
  if (options->RendPostPeriod < MIN_REND_POST_PERIOD) {
    log(LOG_WARN,LD_CONFIG,"RendPostPeriod option must be at least %d seconds."
        " Clipping.", MIN_REND_POST_PERIOD);
    options->RendPostPeriod = MIN_REND_POST_PERIOD;
  }

  if (options->DirPort && ! options->AuthoritativeDir) {
    if (options->DirFetchPeriod > MAX_CACHE_DIR_FETCH_PERIOD) {
      log(LOG_WARN, LD_CONFIG, "Caching directory servers must have "
          "DirFetchPeriod less than %d seconds. Clipping.",
          MAX_CACHE_DIR_FETCH_PERIOD);
      options->DirFetchPeriod = MAX_CACHE_DIR_FETCH_PERIOD;
    }
    if (options->StatusFetchPeriod > MAX_CACHE_STATUS_FETCH_PERIOD) {
      log(LOG_WARN, LD_CONFIG, "Caching directory servers must have "
          "StatusFetchPeriod less than %d seconds. Clipping.",
          MAX_CACHE_STATUS_FETCH_PERIOD);
      options->StatusFetchPeriod = MAX_CACHE_STATUS_FETCH_PERIOD;
    }
  }

  if (options->DirFetchPeriod > MAX_DIR_PERIOD) {
    log(LOG_WARN, LD_CONFIG, "DirFetchPeriod is too large; clipping to %ds.",
        MAX_DIR_PERIOD);
    options->DirFetchPeriod = MAX_DIR_PERIOD;
  }
  if (options->StatusFetchPeriod > MAX_DIR_PERIOD) {
    log(LOG_WARN, LD_CONFIG,"StatusFetchPeriod is too large; clipping to %ds.",
        MAX_DIR_PERIOD);
    options->StatusFetchPeriod = MAX_DIR_PERIOD;
  }
  if (options->RendPostPeriod > MAX_DIR_PERIOD) {
    log(LOG_WARN, LD_CONFIG, "RendPostPeriod is too large; clipping to %ds.",
        MAX_DIR_PERIOD);
    options->RendPostPeriod = MAX_DIR_PERIOD;
  }

  if (options->KeepalivePeriod < 1)
    REJECT("KeepalivePeriod option must be positive.");

  if (options->BandwidthRate > INT_MAX) {
    log(LOG_WARN,LD_CONFIG,"BandwidthRate must be less than %d",INT_MAX);
    result = -1;
  }
  if (options->BandwidthBurst > INT_MAX) {
    log(LOG_WARN,LD_CONFIG,"BandwidthBurst must be less than %d",INT_MAX);
    result = -1;
  }
  if (server_mode(options) &&
      options->BandwidthRate < ROUTER_REQUIRED_MIN_BANDWIDTH*2) {
    log(LOG_WARN,LD_CONFIG,"BandwidthRate is set to %d bytes/second. "
        "For servers, it must be at least %d.",
        (int)options->BandwidthRate, ROUTER_REQUIRED_MIN_BANDWIDTH*2);
    result = -1;
  }
  if (options->BandwidthRate > options->BandwidthBurst)
    REJECT("BandwidthBurst must be at least equal to BandwidthRate.");

  if (accounting_parse_options(options, 1)<0) {
    result = -1;
  }

  if (options->HttpProxy) { /* parse it now */
    if (parse_addr_port(options->HttpProxy, NULL,
                        &options->HttpProxyAddr, &options->HttpProxyPort) < 0)
      REJECT("HttpProxy failed to parse or resolve. Please fix.");
    if (options->HttpProxyPort == 0) { /* give it a default */
      options->HttpProxyPort = 80;
    }
  }

  if (options->HttpProxyAuthenticator) {
    if (strlen(options->HttpProxyAuthenticator) >= 48)
      REJECT("HttpProxyAuthenticator is too long (>= 48 chars).");
  }

  if (options->HttpsProxy) { /* parse it now */
    if (parse_addr_port(options->HttpsProxy, NULL,
                        &options->HttpsProxyAddr, &options->HttpsProxyPort) <0)
      REJECT("HttpsProxy failed to parse or resolve. Please fix.");
    if (options->HttpsProxyPort == 0) { /* give it a default */
      options->HttpsProxyPort = 443;
    }
  }

  if (options->HttpsProxyAuthenticator) {
    if (strlen(options->HttpsProxyAuthenticator) >= 48)
      REJECT("HttpsProxyAuthenticator is too long (>= 48 chars).");
  }

  if (options->HashedControlPassword) {
    if (decode_hashed_password(NULL, options->HashedControlPassword)<0)
      REJECT("Bad HashedControlPassword: wrong length or bad encoding");
  }
  if (options->HashedControlPassword && options->CookieAuthentication)
    REJECT("Cannot set both HashedControlPassword and CookieAuthentication");

  if (options->UseEntryGuards && ! options->NumEntryGuards)
    REJECT("Cannot enable UseEntryGuards with NumEntryGuards set to 0");

  if (check_nickname_list(options->ExitNodes, "ExitNodes"))
    result = -1;
  if (check_nickname_list(options->EntryNodes, "EntryNodes"))
    result = -1;
  if (check_nickname_list(options->ExcludeNodes, "ExcludeNodes"))
    result = -1;
  if (check_nickname_list(options->RendNodes, "RendNodes"))
    result = -1;
  if (check_nickname_list(options->RendNodes, "RendExcludeNodes"))
    result = -1;
  if (check_nickname_list(options->MyFamily, "MyFamily"))
    result = -1;
  for (cl = options->NodeFamilies; cl; cl = cl->next) {
    if (check_nickname_list(cl->value, "NodeFamily"))
      result = -1;
  }

  if (config_parse_addr_policy(options->ExitPolicy, &addr_policy, -1))
    REJECT("Error in Exit Policy entry.");

  options_append_default_exit_policy(&addr_policy);
  if (server_mode(options)) {
    exit_policy_implicitly_allows_local_networks(addr_policy, 1);
  }
  /* The rest of these calls *append* to addr_policy. So don't actually
   * use the results for anything other than checking if they parse! */
  if (config_parse_addr_policy(options->DirPolicy, &addr_policy, -1))
    REJECT("Error in DirPolicy entry.");
  if (config_parse_addr_policy(options->SocksPolicy, &addr_policy, -1))
    REJECT("Error in SocksPolicy entry.");
  if (config_parse_addr_policy(options->ReachableAddresses, &addr_policy,
                               ADDR_POLICY_ACCEPT))
    REJECT("Error in ReachableAddresses entry.");
  if (config_parse_addr_policy(options->AuthDirReject, &addr_policy,
                               ADDR_POLICY_REJECT))
    REJECT("Error in ReachableAddresses entry.");
  if (config_parse_addr_policy(options->AuthDirInvalid, &addr_policy,
                               ADDR_POLICY_REJECT))
    REJECT("Error in ReachableAddresses entry.");

  addr_policy_free(addr_policy);

  for (cl = options->RedirectExit; cl; cl = cl->next) {
    if (parse_redirect_line(NULL, cl)<0)
      result = -1;
  }

  if (options->DirServers) {
    if (!old_options ||
        !config_lines_eq(options->DirServers, old_options->DirServers))
        COMPLAIN("You have used DirServer to specify directory authorities in "
                 "your configuration.  This is potentially dangerous: it can "
                 "make you look different from all other Tor users, and hurt "
                 "your anonymity.  Even if you've specified the same "
                 "authorities as Tor uses by default, the defaults could "
                 "change in the future.  Be sure you know what you're doing.");
    for (cl = options->DirServers; cl; cl = cl->next) {
      if (parse_dir_server_line(cl->value, 1)<0)
        result = -1;
    }
  }

  if (rend_config_services(options, 1) < 0)
    result = -1;

  return result;
#undef REJECT
#undef COMPLAIN
}

/** Helper: return true iff s1 and s2 are both NULL, or both non-NULL
 * equal strings. */
static int
opt_streq(const char *s1, const char *s2)
{
  if (!s1 && !s2)
    return 1;
  else if (s1 && s2 && !strcmp(s1,s2))
    return 1;
  else
    return 0;
}

/** Check if any of the previous options have changed but aren't allowed to. */
static int
options_transition_allowed(or_options_t *old, or_options_t *new_val)
{
  if (!old)
    return 0;

  if (!opt_streq(old->PidFile, new_val->PidFile)) {
    warn(LD_CONFIG,"PidFile is not allowed to change. Failing.");
    return -1;
  }

  if (old->RunAsDaemon != new_val->RunAsDaemon) {
    warn(LD_CONFIG,"While Tor is running, changing RunAsDaemon is not allowed."
         " Failing.");
    return -1;
  }

  if (strcmp(old->DataDirectory,new_val->DataDirectory)!=0) {
    warn(LD_CONFIG,"While Tor is running, changing DataDirectory "
         "(\"%s\"->\"%s\") is not allowed. Failing.",
         old->DataDirectory, new_val->DataDirectory);
    return -1;
  }

  if (!opt_streq(old->User, new_val->User)) {
    warn(LD_CONFIG,"While Tor is running, changing User is not allowed. "
         "Failing.");
    return -1;
  }

  if (!opt_streq(old->Group, new_val->Group)) {
    warn(LD_CONFIG,"While Tor is running, changing Group is not allowed. "
         "Failing.");
    return -1;
  }

  if (old->HardwareAccel != new_val->HardwareAccel) {
    warn(LD_CONFIG,"While Tor is running, changing HardwareAccel is not "
         "allowed. Failing.");
    return -1;
  }

  return 0;
}

/** Return 1 if any change from <b>old_options</b> to <b>new_options</b>
 * will require us to rotate the cpu and dns workers; else return 0. */
static int
options_transition_affects_workers(or_options_t *old_options,
                                   or_options_t *new_options)
{
  if (!opt_streq(old_options->DataDirectory, new_options->DataDirectory) ||
      old_options->NumCpus != new_options->NumCpus ||
      old_options->ORPort != new_options->ORPort ||
      old_options->SafeLogging != new_options->SafeLogging ||
      !config_lines_eq(old_options->Logs, new_options->Logs))
    return 1;

  /* Check whether log options match. */

  /* Nothing that changed matters. */
  return 0;
}

/** Return 1 if any change from <b>old_options</b> to <b>new_options</b>
 * will require us to generate a new descriptor; else return 0. */
static int
options_transition_affects_descriptor(or_options_t *old_options,
                                      or_options_t *new_options)
{
  if (!opt_streq(old_options->DataDirectory, new_options->DataDirectory) ||
      !opt_streq(old_options->Nickname,new_options->Nickname) ||
      !opt_streq(old_options->Address,new_options->Address) ||
      !config_lines_eq(old_options->ExitPolicy,new_options->ExitPolicy) ||
      old_options->ORPort != new_options->ORPort ||
      old_options->DirPort != new_options->DirPort ||
      old_options->ClientOnly != new_options->ClientOnly ||
      old_options->NoPublish != new_options->NoPublish ||
      old_options->BandwidthRate != new_options->BandwidthRate ||
      old_options->BandwidthBurst != new_options->BandwidthBurst ||
      !opt_streq(old_options->ContactInfo, new_options->ContactInfo) ||
      !opt_streq(old_options->MyFamily, new_options->MyFamily) ||
      !opt_streq(old_options->AccountingStart, new_options->AccountingStart) ||
      old_options->AccountingMax != new_options->AccountingMax)
    return 1;

  return 0;
}

#ifdef MS_WINDOWS
/** Return the directory on windows where we expect to find our application
 * data. */
static char *
get_windows_conf_root(void)
{
  static int is_set = 0;
  static char path[MAX_PATH+1];

  LPITEMIDLIST idl;
  IMalloc *m;
  HRESULT result;

  if (is_set)
    return path;

  /* Find X:\documents and settings\username\application data\ .
   * We would use SHGetSpecialFolder path, but that wasn't added until IE4.
   */
  if (!SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA,
                                            &idl))) {
    GetCurrentDirectory(MAX_PATH, path);
    is_set = 1;
    warn(LD_CONFIG, "I couldn't find your application data folder: are you "
         "running an ancient version of Windows 95? Defaulting to \"%s\"",
         path);
    return path;
  }
  /* Convert the path from an "ID List" (whatever that is!) to a path. */
  result = SHGetPathFromIDList(idl, path);
  /* Now we need to free the */
  SHGetMalloc(&m);
  if (m) {
    m->lpVtbl->Free(m, idl);
    m->lpVtbl->Release(m);
  }
  if (!SUCCEEDED(result)) {
    return NULL;
  }
  strlcat(path,"\\tor",MAX_PATH);
  is_set = 1;
  return path;
}
#endif

/** Return the default location for our torrc file. */
static const char *
get_default_conf_file(void)
{
#ifdef MS_WINDOWS
  static char path[MAX_PATH+1];
  strlcpy(path, get_windows_conf_root(), MAX_PATH);
  strlcat(path,"\\torrc",MAX_PATH);
  return path;
#else
  return (CONFDIR "/torrc");
#endif
}

/** Verify whether lst is a string containing valid-looking space-separated
 * nicknames, or NULL. Return 0 on success. Warn and return -1 on failure.
 */
static int
check_nickname_list(const char *lst, const char *name)
{
  int r = 0;
  smartlist_t *sl;

  if (!lst)
    return 0;
  sl = smartlist_create();
  smartlist_split_string(sl, lst, ",", SPLIT_SKIP_SPACE|SPLIT_IGNORE_BLANK, 0);
  SMARTLIST_FOREACH(sl, const char *, s,
    {
      if (!is_legal_nickname_or_hexdigest(s)) {
        warn(LD_CONFIG, "Invalid nickname '%s' in %s line", s, name);
        r = -1;
      }
    });
  SMARTLIST_FOREACH(sl, char *, s, tor_free(s));
  smartlist_free(sl);
  return r;
}

/** Read a configuration file into <b>options</b>, finding the configuration
 * file location based on the command line.  After loading the options,
 * validate them for consistency, then take actions based on them.
 * Return 0 if success, -1 if failure. */
int
options_init_from_torrc(int argc, char **argv)
{
  or_options_t *oldoptions, *newoptions;
  config_line_t *cl;
  char *cf=NULL, *fname=NULL;
  int i, retval;
  int using_default_torrc;
  static char **backup_argv;
  static int backup_argc;

  if (argv) { /* first time we're called. save commandline args */
    backup_argv = argv;
    backup_argc = argc;
    oldoptions = NULL;
  } else { /* we're reloading. need to clean up old options first. */
    argv = backup_argv;
    argc = backup_argc;
    oldoptions = get_options();
  }
  if (argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1],"--help"))) {
    print_usage();
    exit(0);
  }

  if (argc > 1 && (!strcmp(argv[1],"--version"))) {
    printf("Tor version %s.\n",VERSION);
    if (argc > 2 && (!strcmp(argv[2],"--version"))) {
      print_cvs_version();
    }
    exit(0);
  }

  newoptions = tor_malloc_zero(sizeof(or_options_t));
  newoptions->_magic = OR_OPTIONS_MAGIC;
  options_init(newoptions);

  /* learn config file name */
  fname = NULL;
  using_default_torrc = 1;
  newoptions->command = CMD_RUN_TOR;
  for (i = 1; i < argc; ++i) {
    if (i < argc-1 && !strcmp(argv[i],"-f")) {
      if (fname) {
        log(LOG_WARN, LD_CONFIG, "Duplicate -f options on command line.");
        tor_free(fname);
      }
      fname = tor_strdup(argv[i+1]);
      using_default_torrc = 0;
      ++i;
    } else if (!strcmp(argv[i],"--list-fingerprint")) {
      newoptions->command = CMD_LIST_FINGERPRINT;
    } else if (!strcmp(argv[i],"--hash-password")) {
      newoptions->command = CMD_HASH_PASSWORD;
      newoptions->command_arg = tor_strdup( (i < argc-1) ? argv[i+1] : "");
      ++i;
    } else if (!strcmp(argv[i],"--verify-config")) {
      newoptions->command = CMD_VERIFY_CONFIG;
    }
  }
  if (using_default_torrc) {
    /* didn't find one, try CONFDIR */
    const char *dflt = get_default_conf_file();
    char *fn = NULL;
    if (dflt && file_status(dflt) == FN_FILE) {
      fname = tor_strdup(dflt);
    } else {
#ifndef MS_WINDOWS
      fn = expand_filename("~/.torrc");
      if (fn && file_status(fn) == FN_FILE) {
        fname = fn;
      } else {
        tor_free(fn);
        fname = tor_strdup(dflt);
      }
#else
      fname = tor_strdup(dflt);
#endif
    }
  }
  tor_assert(fname);
  log(LOG_DEBUG, LD_CONFIG, "Opening config file \"%s\"", fname);

  tor_free(torrc_fname);
  torrc_fname = fname;

  /* get config lines, assign them */
  if (file_status(fname) != FN_FILE ||
      !(cf = read_file_to_str(fname,0))) {
    if (using_default_torrc == 1) {
      log(LOG_NOTICE, LD_CONFIG, "Configuration file \"%s\" not present, "
          "using reasonable defaults.", fname);
      tor_free(fname); /* sets fname to NULL */
      torrc_fname = NULL;
    } else {
      log(LOG_WARN, LD_CONFIG,
          "Unable to open configuration file \"%s\".", fname);
      goto err;
    }
  } else { /* it opened successfully. use it. */
    retval = config_get_lines(cf, &cl);
    tor_free(cf);
    if (retval < 0)
      goto err;
    retval = config_assign(&options_format, newoptions, cl, 0, 0);
    config_free_lines(cl);
    if (retval < 0)
      goto err;
  }

  /* Go through command-line variables too */
  if (config_get_commandlines(argc, argv, &cl) < 0)
    goto err;
  retval = config_assign(&options_format, newoptions, cl, 0, 0);
  config_free_lines(cl);
  if (retval < 0)
    goto err;

  /* Validate newoptions */
  if (options_validate(oldoptions, newoptions, 0) < 0)
    goto err;

  if (options_transition_allowed(oldoptions, newoptions) < 0)
    goto err;

  if (set_options(newoptions))
    goto err; /* frees and replaces old options */
  return 0;
 err:
  tor_free(fname);
  torrc_fname = NULL;
  config_free(&options_format, newoptions);
  return -1;
}

/** Return the location for our configuration file.
 */
const char *
get_torrc_fname(void)
{
  if (torrc_fname)
    return torrc_fname;
  else
    return get_default_conf_file();
}

/** Adjust the address map mased on the MapAddress elements in the
 * configuration <b>options</b>
 */
static void
config_register_addressmaps(or_options_t *options)
{
  smartlist_t *elts;
  config_line_t *opt;
  char *from, *to;

  addressmap_clear_configured();
  elts = smartlist_create();
  for (opt = options->AddressMap; opt; opt = opt->next) {
    smartlist_split_string(elts, opt->value, NULL,
                           SPLIT_SKIP_SPACE|SPLIT_IGNORE_BLANK, 2);
    if (smartlist_len(elts) >= 2) {
      from = smartlist_get(elts,0);
      to = smartlist_get(elts,1);
      if (!is_plausible_address(from)) {
        warn(LD_CONFIG,"Skipping invalid argument '%s' to MapAddress",from);
      } else if (!is_plausible_address(to)) {
        warn(LD_CONFIG,"Skipping invalid argument '%s' to MapAddress",to);
      } else {
        addressmap_register(from, tor_strdup(to), 0);
        if (smartlist_len(elts)>2) {
          warn(LD_CONFIG,"Ignoring extra arguments to MapAddress.");
        }
      }
    } else {
      warn(LD_CONFIG,"MapAddress '%s' has too few arguments. Ignoring.",
           opt->value);
    }
    SMARTLIST_FOREACH(elts, char*, cp, tor_free(cp));
    smartlist_clear(elts);
  }
  smartlist_free(elts);
}

/** If <b>range</b> is of the form MIN-MAX, for MIN and MAX both
 * recognized log severity levels, set *<b>min_out</b> to MIN and
 * *<b>max_out</b> to MAX and return 0.  Else, if <b>range</b> is of
 * the form MIN, act as if MIN-err had been specified.  Else, warn and
 * return -1.
 */
static int
parse_log_severity_range(const char *range, int *min_out, int *max_out)
{
  int levelMin, levelMax;
  const char *cp;
  cp = strchr(range, '-');
  if (cp) {
    if (cp == range) {
      levelMin = LOG_DEBUG;
    } else {
      char *tmp_sev = tor_strndup(range, cp - range);
      levelMin = parse_log_level(tmp_sev);
      if (levelMin < 0) {
        warn(LD_CONFIG, "Unrecognized log severity '%s': must be one of "
             "err|warn|notice|info|debug", tmp_sev);
        tor_free(tmp_sev);
        return -1;
      }
      tor_free(tmp_sev);
    }
    if (!*(cp+1)) {
      levelMax = LOG_ERR;
    } else {
      levelMax = parse_log_level(cp+1);
      if (levelMax < 0) {
        warn(LD_CONFIG, "Unrecognized log severity '%s': must be one of "
             "err|warn|notice|info|debug", cp+1);
        return -1;
      }
    }
  } else {
    levelMin = parse_log_level(range);
    if (levelMin < 0) {
      warn(LD_CONFIG, "Unrecognized log severity '%s': must be one of "
           "err|warn|notice|info|debug", range);
      return -1;
    }
    levelMax = LOG_ERR;
  }

  *min_out = levelMin;
  *max_out = levelMax;

  return 0;
}

/** Try to convert a pair of old-style logging options [LogLevel, and
 * (LogFile/Syslog)] to a new-style option, and add the new option to
 * options->Logs. */
static int
convert_log_option(or_options_t *options, config_line_t *level_opt,
                   config_line_t *file_opt, int isDaemon)
{
  int levelMin = -1, levelMax = -1;

  if (level_opt) {
    if (parse_log_severity_range(level_opt->value, &levelMin, &levelMax))
      return -1;
  }
  if (levelMin < 0 && levelMax < 0) {
    levelMin = LOG_NOTICE;
    levelMax = LOG_ERR;
  } else if (levelMin < 0) {
    levelMin = levelMax;
  } else {
    levelMax = LOG_ERR;
  }

  if (file_opt && !strcasecmp(file_opt->key, "LogFile")) {
    if (add_single_log_option(options, levelMin, levelMax, "file",
                              file_opt->value) < 0) {
      warn(LD_FS, "Cannot write to LogFile \"%s\": %s.", file_opt->value,
           strerror(errno));
      return -1;
    }
  } else if (file_opt && !strcasecmp(file_opt->key, "SysLog")) {
    if (add_single_log_option(options, levelMin, levelMax, "syslog", NULL) < 0)
      return -1;
  } else if (!isDaemon) {
    add_single_log_option(options, levelMin, levelMax, "stdout", NULL);
  }
  return 0;
}

/**
 * Initialize the logs based on the configuration file.
 */
int
options_init_logs(or_options_t *options, int validate_only)
{
  config_line_t *opt;
  int ok;
  smartlist_t *elts;
  int daemon = options->RunAsDaemon;

  ok = 1;
  elts = smartlist_create();

  for (opt = options->Logs; opt; opt = opt->next) {
    int levelMin=LOG_DEBUG, levelMax=LOG_ERR;
    smartlist_split_string(elts, opt->value, NULL,
                           SPLIT_SKIP_SPACE|SPLIT_IGNORE_BLANK, 3);
    if (smartlist_len(elts) == 0) {
      warn(LD_CONFIG, "Bad syntax on Log option 'Log %s'", opt->value);
      ok = 0; goto cleanup;
    }
    if (parse_log_severity_range(smartlist_get(elts,0), &levelMin,
                                 &levelMax)) {
      ok = 0; goto cleanup;
    }
    if (smartlist_len(elts) < 2) { /* only loglevels were provided */
      if (!validate_only) {
        if (daemon) {
          warn(LD_CONFIG, "Can't log to stdout with RunAsDaemon set.");
          ok = 0; goto cleanup;
        }
        add_stream_log(levelMin, levelMax, "<stdout>", stdout);
      }
      goto cleanup;
    }
    if (!strcasecmp(smartlist_get(elts,1), "file")) {
      if (smartlist_len(elts) != 3) {
        warn(LD_CONFIG, "Bad syntax on Log option 'Log %s'", opt->value);
        ok = 0; goto cleanup;
      }
      if (!validate_only) {
        if (add_file_log(levelMin, levelMax, smartlist_get(elts, 2)) < 0) {
          warn(LD_CONFIG, "Couldn't open file for 'Log %s'", opt->value);
          ok = 0;
        }
      }
      goto cleanup;
    }
    if (smartlist_len(elts) != 2) {
      warn(LD_CONFIG, "Bad syntax on Log option 'Log %s'", opt->value);
      ok = 0; goto cleanup;
    }
    if (!strcasecmp(smartlist_get(elts,1), "stdout")) {
      if (daemon) {
        warn(LD_CONFIG, "Can't log to stdout with RunAsDaemon set.");
        ok = 0; goto cleanup;
      }
      if (!validate_only) {
        add_stream_log(levelMin, levelMax, "<stdout>", stdout);
      }
    } else if (!strcasecmp(smartlist_get(elts,1), "stderr")) {
      if (daemon) {
        warn(LD_CONFIG, "Can't log to stdout with RunAsDaemon set.");
        ok = 0; goto cleanup;
      }
      if (!validate_only) {
        add_stream_log(levelMin, levelMax, "<stderr>", stderr);
      }
    } else if (!strcasecmp(smartlist_get(elts,1), "syslog")) {
#ifdef HAVE_SYSLOG_H
      if (!validate_only)
        add_syslog_log(levelMin, levelMax);
#else
      warn(LD_CONFIG, "Syslog is not supported in this compilation.");
#endif
    } else {
      warn(LD_CONFIG, "Unrecognized log type %s",
           (const char*)smartlist_get(elts,1));
      if (strchr(smartlist_get(elts,1), '/') ||
          strchr(smartlist_get(elts,1), '\\')) {
        warn(LD_CONFIG, "Did you mean to say 'Log file %s' ?",
             (const char *)smartlist_get(elts,1));
      }
      ok = 0; goto cleanup;
    }
  cleanup:
    SMARTLIST_FOREACH(elts, char*, cp, tor_free(cp));
    smartlist_clear(elts);
  }
  smartlist_free(elts);

  return ok?0:-1;
}

/** Add a single option of the form Log min-max \<type\> [fname] to options. */
static int
add_single_log_option(or_options_t *options, int minSeverity, int maxSeverity,
                      const char *type, const char *fname)
{
  char *buf;
  size_t len;

  len = 256 + fname?strlen(fname):0;
  buf = tor_malloc(len);

  if (tor_snprintf(buf, len, "%s%s%s %s%s%s",
                log_level_to_string(minSeverity),
                maxSeverity == LOG_ERR ? "" : "-",
                maxSeverity == LOG_ERR ? "" : log_level_to_string(maxSeverity),
                type, fname?" ":"", fname?fname:"")<0) {
    warn(LD_BUG, "Normalized log option too long.");
    tor_free(buf);
    return -1;
  }

  log(LOG_WARN, LD_CONFIG, "The old LogLevel/LogFile/DebugLogFile/SysLog "
      "options are deprecated, and will go away soon.  Your new torrc line "
      "should be: 'Log %s'", buf);
  config_line_append(&options->Logs, "Log", buf);
  tor_free(buf);
  return 0;
}

/** Convert all old-style logging options to new-style Log options. Return 0
 * on success, -1 on failure. */
static int
normalize_log_options(or_options_t *options)
{
  /* The order of options is:  Level? (File Level?)+
   */
  config_line_t *opt = options->OldLogOptions;

  /* Special case for if first option is LogLevel. */
  if (opt && !strcasecmp(opt->key, "LogLevel")) {
    if (opt->next && (!strcasecmp(opt->next->key, "LogFile") ||
                      !strcasecmp(opt->next->key, "SysLog"))) {
      if (convert_log_option(options, opt, opt->next, options->RunAsDaemon)< 0)
        return -1;
      opt = opt->next->next;
    } else if (!opt->next) {
      if (convert_log_option(options, opt, NULL, options->RunAsDaemon) < 0)
        return -1;
      opt = opt->next;
    } else {
      ; /* give warning below */
    }
  }

  while (opt) {
    if (!strcasecmp(opt->key, "LogLevel")) {
      warn(LD_CONFIG, "Two LogLevel options in a row without "
           "intervening LogFile or SysLog");
      opt = opt->next;
    } else {
      tor_assert(!strcasecmp(opt->key, "LogFile") ||
                 !strcasecmp(opt->key, "SysLog"));
      if (opt->next && !strcasecmp(opt->next->key, "LogLevel")) {
        /* LogFile/SysLog followed by LogLevel */
        if (convert_log_option(options,opt->next,opt, options->RunAsDaemon) <0)
          return -1;
        opt = opt->next->next;
      } else {
        /* LogFile/SysLog followed by LogFile/SysLog or end of list. */
        if (convert_log_option(options,NULL, opt, options->RunAsDaemon) < 0)
          return -1;
        opt = opt->next;
      }
    }
  }

  if (options->DebugLogFile) {
    if (add_single_log_option(options, LOG_DEBUG, LOG_ERR, "file",
                              options->DebugLogFile) < 0)
      return -1;
  }

  tor_free(options->DebugLogFile);
  config_free_lines(options->OldLogOptions);
  options->OldLogOptions = NULL;

  return 0;
}

#define DEFAULT_EXIT_POLICY                                                  \
  "reject private:*,reject *:25,reject *:119,reject *:135-139,reject *:445," \
  "reject *:465,reject *:587,reject *:1214,reject *:4661-4666,"              \
  "reject *:6346-6429,reject *:6699,reject *:6881-6999,accept *:*"

/** Add the default exit policy entries to <b>policy</b>
 */
void
options_append_default_exit_policy(addr_policy_t **policy)
{
  config_line_t tmp;
  addr_policy_t *ap;

  tmp.key = NULL;
  tmp.value = (char*)DEFAULT_EXIT_POLICY;
  tmp.next = NULL;
  config_parse_addr_policy(&tmp, policy, -1);

  /* Remove redundant parts, if any. */
  for (ap=*policy; ap; ap=ap->next) {
    if (ap->msk == 0 && ap->prt_min <= 1 && ap->prt_max >= 65535) {
      if (ap->next) {
        addr_policy_free(ap->next);
        ap->next = NULL;
      }
      return;
    }
  }
}

static int
config_expand_exit_policy_aliases(smartlist_t *entries, int assume_action)
{
  static const char *prefixes[] = {
    "0.0.0.0/8", "169.254.0.0/16",
    "127.0.0.0/8", "192.168.0.0/16", "10.0.0.0/8", "172.16.0.0/12",NULL };
  int i;
  char *pre=NULL, *post=NULL;
  int expanded_any = 0;
  pre = smartlist_join_strings(entries,",",0,NULL);
  for (i = 0; i < smartlist_len(entries); ++i) {
    char *v = smartlist_get(entries, i);
    const char *cp, *ports;
    const char *action;
    int prefix_idx;
    if (!strcasecmpstart(v, "accept")) {
      action = "accept ";
      cp = v+strlen("accept");
    } else if (!strcasecmpstart(v, "reject")) {
      action = "reject ";
      cp = v+strlen("reject");
    } else if (assume_action >= 0) {
      action = "";
      cp = v;
    } else {
      warn(LD_CONFIG,"Policy '%s' didn't start with accept or reject.", v);
      tor_free(pre);
      return -1;
    }
    cp = eat_whitespace(cp);
    if (strcmpstart(cp, "private"))
      continue; /* No need to expand. */
    cp += strlen("private");
    cp = eat_whitespace(cp);
    if (*cp && *cp != ':')
      continue; /* It wasn't "private" after all. */
    ports = cp;
    /* Okay. We're going to replace entries[i] with a bunch of new entries,
     * in order. */
    smartlist_del_keeporder(entries, i);
    for (prefix_idx = 0; prefixes[prefix_idx]; ++prefix_idx) {
      size_t replacement_len = 16+strlen(prefixes[prefix_idx])+strlen(ports);
      char *replacement = tor_malloc(replacement_len);
      tor_snprintf(replacement, replacement_len, "%s%s%s",
                   action, prefixes[prefix_idx], ports);
      smartlist_insert(entries, i++, replacement);
    }
    tor_free(v);
    expanded_any = 1;
    --i;
  }
  post = smartlist_join_strings(entries,",",0,NULL);
  if (expanded_any)
    info(LD_CONFIG, "Expanded '%s' to '%s'", pre, post);
  tor_free(pre);
  tor_free(post);
  return expanded_any;
}

/**
 * Given a linked list of config lines containing "allow" and "deny" tokens,
 * parse them and append the result to <b>dest</b>.  Return -1 if any tokens
 * are malformed, else return 0.
 */
int
config_parse_addr_policy(config_line_t *cfg,
                         addr_policy_t **dest,
                         int assume_action)
{
  addr_policy_t **nextp;
  smartlist_t *entries;
  int r = 0;

  if (!cfg)
    return 0;

  nextp = dest;

  while (*nextp)
    nextp = &((*nextp)->next);

  entries = smartlist_create();
  for (; cfg; cfg = cfg->next) {
    smartlist_split_string(entries, cfg->value, ",",
                           SPLIT_SKIP_SPACE|SPLIT_IGNORE_BLANK, 0);
    if (config_expand_exit_policy_aliases(entries,assume_action)<0) {
      r = -1;
      continue;
    }
    SMARTLIST_FOREACH(entries, const char *, ent,
    {
      debug(LD_CONFIG,"Adding new entry '%s'",ent);
      *nextp = router_parse_addr_policy_from_string(ent, assume_action);
      if (*nextp) {
        nextp = &((*nextp)->next);
      } else {
        warn(LD_CONFIG,"Malformed policy '%s'.", ent);
        r = -1;
      }
    });
    SMARTLIST_FOREACH(entries, char *, ent, tor_free(ent));
    smartlist_clear(entries);
  }
  smartlist_free(entries);
  return r;
}

/** Compare two provided address policies, and return -1, 0, or 1 if the first
 * is less than, equal to, or greater than the second. */
int
config_cmp_addr_policies(addr_policy_t *a, addr_policy_t *b)
{
  int r;
  while (a && b) {
    if ((r=((int)a->addr - (int)b->addr)))
      return r;
    if ((r=((int)a->msk - (int)b->msk)))
      return r;
    if ((r=((int)a->prt_min - (int)b->prt_min)))
      return r;
    if ((r=((int)a->prt_max - (int)b->prt_max)))
      return r;
    a = a->next;
    b = b->next;
  }
  if (!a && !b)
    return 0;
  if (a)
    return -1;
  else
    return 1;
}

/** Release all storage held by <b>p</b> */
void
addr_policy_free(addr_policy_t *p)
{
  addr_policy_t *e;

  while (p) {
    e = p;
    p = p->next;
    tor_free(e->string);
    tor_free(e);
  }
}

/** Parse a single RedirectExit line's contents from <b>line</b>.  If
 *  they are valid, and <b>result</b> is not NULL, add an element to
 *  <b>result</b> and return 0. Else if they are valid, return 0.
 *  Else return -1. */
static int
parse_redirect_line(smartlist_t *result, config_line_t *line)
{
  smartlist_t *elements = NULL;
  exit_redirect_t *r;

  tor_assert(line);

  r = tor_malloc_zero(sizeof(exit_redirect_t));
  elements = smartlist_create();
  smartlist_split_string(elements, line->value, NULL,
                         SPLIT_SKIP_SPACE|SPLIT_IGNORE_BLANK, 0);
  if (smartlist_len(elements) != 2) {
    warn(LD_CONFIG, "Wrong number of elements in RedirectExit line");
    goto err;
  }
  if (parse_addr_and_port_range(smartlist_get(elements,0),&r->addr,&r->mask,
                                &r->port_min,&r->port_max)) {
    warn(LD_CONFIG, "Error parsing source address in RedirectExit line");
    goto err;
  }
  if (0==strcasecmp(smartlist_get(elements,1), "pass")) {
    r->is_redirect = 0;
  } else {
    if (parse_addr_port(smartlist_get(elements,1),NULL,&r->addr_dest,
                             &r->port_dest)) {
      warn(LD_CONFIG, "Error parsing dest address in RedirectExit line");
      goto err;
    }
    r->is_redirect = 1;
  }

  goto done;
 err:
  tor_free(r);
 done:
  SMARTLIST_FOREACH(elements, char *, cp, tor_free(cp));
  smartlist_free(elements);
  if (r) {
    if (result)
      smartlist_add(result, r);
    else
      tor_free(r);
    return 0;
  } else {
    return -1;
  }
}

/** Read the contents of a DirServer line from <b>line</b>.  Return 0
 * if the line is well-formed, and -1 if it isn't.  If
 * <b>validate_only</b> is 0, and the line is well-formed, then add
 * the dirserver described in the line as a valid server. */
static int
parse_dir_server_line(const char *line, int validate_only)
{
  smartlist_t *items = NULL;
  int r, idx;
  char *addrport=NULL, *address=NULL, *nickname=NULL, *fingerprint=NULL;
  uint16_t port;
  char digest[DIGEST_LEN];
  int supports_v1 = 1; /*XXXX011 change default when clients support v2. */

  items = smartlist_create();
  smartlist_split_string(items, line, NULL,
                         SPLIT_SKIP_SPACE|SPLIT_IGNORE_BLANK, -1);
  if (smartlist_len(items) < 2) {
    warn(LD_CONFIG, "Too few arguments to DirServer line.");
    goto err;
  }

  idx = 0;
  if (is_legal_nickname(smartlist_get(items, 0))) {
    nickname = smartlist_get(items, 0);
    smartlist_del_keeporder(items, 0);
  }

  if (!strcmp(smartlist_get(items, 0), "v1")) {
    char *v1 = smartlist_get(items, 0);
    tor_free(v1);
    supports_v1 = 1;
    smartlist_del_keeporder(items, 0);
  }

  if (smartlist_len(items) < 2) {
    warn(LD_CONFIG, "Too few arguments to DirServer line.");
    goto err;
  }
  addrport = smartlist_get(items, 0);
  if (parse_addr_port(addrport, &address, NULL, &port)<0) {
    warn(LD_CONFIG, "Error parsing DirServer address '%s'", addrport);
    goto err;
  }
  if (!port) {
    warn(LD_CONFIG, "Missing port in DirServer address '%s'",addrport);
    goto err;
  }
  smartlist_del_keeporder(items, 0);

  fingerprint = smartlist_join_strings(items, "", 0, NULL);
  if (strlen(fingerprint) != HEX_DIGEST_LEN) {
    warn(LD_CONFIG, "Key digest for DirServer is wrong length.");
    goto err;
  }
  if (base16_decode(digest, DIGEST_LEN, fingerprint, HEX_DIGEST_LEN)<0) {
    warn(LD_CONFIG, "Unable to decode DirServer key digest.");
    goto err;
  }

  if (!validate_only) {
    debug(LD_DIR, "Trusted dirserver at %s:%d (%s)", address, (int)port,
           (char*)smartlist_get(items,1));
    add_trusted_dir_server(nickname, address, port, digest, supports_v1);
  }

  r = 0;
  goto done;

  err:
  r = -1;

  done:
  SMARTLIST_FOREACH(items, char*, s, tor_free(s));
  smartlist_free(items);
  tor_free(addrport);
  tor_free(address);
  tor_free(nickname);
  tor_free(fingerprint);
  return r;
}

/** Adjust the value of options->DataDirectory, or fill it in if it's
 * absent. Return 0 on success, -1 on failure. */
static int
normalize_data_directory(or_options_t *options)
{
#ifdef MS_WINDOWS
  char *p;
  if (options->DataDirectory)
    return 0; /* all set */
  p = tor_malloc(MAX_PATH);
  strlcpy(p,get_windows_conf_root(),MAX_PATH);
  options->DataDirectory = p;
  return 0;
#else
  const char *d = options->DataDirectory;
  if (!d)
    d = "~/.tor";

 if (strncmp(d,"~/",2) == 0) {
   char *fn = expand_filename(d);
   if (!fn) {
     err(LD_CONFIG,"Failed to expand filename \"%s\".", d);
     return -1;
   }
   if (!options->DataDirectory && !strcmp(fn,"/.tor")) {
     /* If our homedir is /, we probably don't want to use it. */
     /* XXXX Default to /var/lib/tor? */
     warn(LD_CONFIG, "Default DataDirectory is \"~/.tor\".  This expands to "
          "\"%s\", which is probably not what you want.  Using \"%s/tor\" "
          "instead", fn, LOCALSTATEDIR);
     tor_free(fn);
     fn = tor_strdup(LOCALSTATEDIR"/tor");

   }
   tor_free(options->DataDirectory);
   options->DataDirectory = fn;
 }
 return 0;
#endif
}

/** Check and normalize the value of options->DataDirectory; return 0 if it
 * sane, -1 otherwise. */
static int
validate_data_directory(or_options_t *options)
{
  if (normalize_data_directory(options) < 0)
    return -1;
  tor_assert(options->DataDirectory);
  if (strlen(options->DataDirectory) > (512-128)) {
    err(LD_CONFIG, "DataDirectory is too long.");
    return -1;
  }
  return 0;
}

/** This string must remain the same forevermore. It is how we
 * recognize that the torrc file doesn't need to be backed up. */
#define GENERATED_FILE_PREFIX "# This file was generated by Tor; " \
  "if you edit it, comments will not be preserved"
/** This string can change; it tries to give the reader an idea
 * that editing this file by hand is not a good plan. */
#define GENERATED_FILE_COMMENT "# If you want a config file with " \
  "comments, look for torrc.orig.1 or similar"

/** Save a configuration file for the configuration in <b>options</b>
 * into the file <b>fname</b>.  If the file already exists, and
 * doesn't begin with GENERATED_FILE_PREFIX, rename it.  Otherwise
 * replace it.  Return 0 on success, -1 on failure. */
static int
write_configuration_file(const char *fname, or_options_t *options)
{
  char *old_val=NULL, *new_val=NULL, *new_conf=NULL;
  int rename_old = 0, r;
  size_t len;

  if (fname) {
    switch (file_status(fname)) {
      case FN_FILE:
        old_val = read_file_to_str(fname, 0);
        if (strcmpstart(old_val, GENERATED_FILE_PREFIX)) {
          rename_old = 1;
        }
        tor_free(old_val);
        break;
      case FN_NOENT:
        break;
      default:
        warn(LD_CONFIG,"Config file \"%s\" is not a file? Failing.", fname);
        return -1;
    }
  }

  if (!(new_conf = options_dump(options, 1))) {
    warn(LD_BUG, "Couldn't get configuration string");
    goto err;
  }

  len = strlen(new_conf)+256;
  new_val = tor_malloc(len);
  tor_snprintf(new_val, len, "%s\n%s\n\n%s",
               GENERATED_FILE_PREFIX, GENERATED_FILE_COMMENT, new_conf);

  if (rename_old) {
    int i = 1;
    size_t fn_tmp_len = strlen(fname)+32;
    char *fn_tmp;
    tor_assert(fn_tmp_len > strlen(fname)); /*check for overflow*/
    fn_tmp = tor_malloc(fn_tmp_len);
    while (1) {
      if (tor_snprintf(fn_tmp, fn_tmp_len, "%s.orig.%d", fname, i)<0) {
        warn(LD_BUG, "tor_snprintf failed inexplicably");
        tor_free(fn_tmp);
        goto err;
      }
      if (file_status(fn_tmp) == FN_NOENT)
        break;
      ++i;
    }
    notice(LD_CONFIG, "Renaming old configuration file to \"%s\"", fn_tmp);
    rename(fname, fn_tmp);
    tor_free(fn_tmp);
  }

  write_str_to_file(fname, new_val, 0);

  r = 0;
  goto done;
 err:
  r = -1;
 done:
  tor_free(new_val);
  tor_free(new_conf);
  return r;
}

/**
 * Save the current configuration file value to disk.  Return 0 on
 * success, -1 on failure.
 **/
int
options_save_current(void)
{
  if (torrc_fname) {
    /* XXX This fails if we can't write to our configuration file.
     *   Arguably, we should try falling back to datadirectory or something.
     *   But just as arguably, we shouldn't. */
    return write_configuration_file(torrc_fname, get_options());
  }
  return write_configuration_file(get_default_conf_file(), get_options());
}

/** Mapping from a unit name to a multiplier for converting that unit into a
 * base unit. */
struct unit_table_t {
  const char *unit;
  uint64_t multiplier;
};

static struct unit_table_t memory_units[] = {
  { "",          1 },
  { "b",         1<< 0 },
  { "byte",      1<< 0 },
  { "bytes",     1<< 0 },
  { "kb",        1<<10 },
  { "kilobyte",  1<<10 },
  { "kilobytes", 1<<10 },
  { "m",         1<<20 },
  { "mb",        1<<20 },
  { "megabyte",  1<<20 },
  { "megabytes", 1<<20 },
  { "gb",        1<<30 },
  { "gigabyte",  1<<30 },
  { "gigabytes", 1<<30 },
  { "tb",        U64_LITERAL(1)<<40 },
  { "terabyte",  U64_LITERAL(1)<<40 },
  { "terabytes", U64_LITERAL(1)<<40 },
  { NULL, 0 },
};

static struct unit_table_t time_units[] = {
  { "",         1 },
  { "second",   1 },
  { "seconds",  1 },
  { "minute",   60 },
  { "minutes",  60 },
  { "hour",     60*60 },
  { "hours",    60*60 },
  { "day",      24*60*60 },
  { "days",     24*60*60 },
  { "week",     7*24*60*60 },
  { "weeks",    7*24*60*60 },
  { NULL, 0 },
};

/** Parse a string <b>val</b> containing a number, zero or more
 * spaces, and an optional unit string.  If the unit appears in the
 * table <b>u</b>, then multiply the number by the unit multiplier.
 * On success, set *<b>ok</b> to 1 and return this product.
 * Otherwise, set *<b>ok</b> to 0.
 */
static uint64_t
config_parse_units(const char *val, struct unit_table_t *u, int *ok)
{
  uint64_t v;
  char *cp;

  tor_assert(ok);

  v = tor_parse_uint64(val, 10, 0, UINT64_MAX, ok, &cp);
  if (!*ok)
    return 0;
  if (!cp) {
    *ok = 1;
    return v;
  }
  while (TOR_ISSPACE(*cp))
    ++cp;
  for ( ;u->unit;++u) {
    if (!strcasecmp(u->unit, cp)) {
      v *= u->multiplier;
      *ok = 1;
      return v;
    }
  }
  warn(LD_CONFIG, "Unknown unit '%s'.", cp);
  *ok = 0;
  return 0;
}

/** Parse a string in the format "number unit", where unit is a unit of
 * information (byte, KB, M, etc).  On success, set *<b>ok</b> to true
 * and return the number of bytes specified.  Otherwise, set
 * *<b>ok</b> to false and return 0. */
static uint64_t
config_parse_memunit(const char *s, int *ok)
{
  return config_parse_units(s, memory_units, ok);
}

/** Parse a string in the format "number unit", where unit is a unit of time.
 * On success, set *<b>ok</b> to true and return the number of seconds in
 * the provided interval.  Otherwise, set *<b>ok</b> to 0 and return -1.
 */
static int
config_parse_interval(const char *s, int *ok)
{
  uint64_t r;
  r = config_parse_units(s, time_units, ok);
  if (!ok)
    return -1;
  if (r > INT_MAX) {
    warn(LD_CONFIG, "Interval '%s' is too long", s);
    *ok = 0;
    return -1;
  }
  return (int)r;
}

/**
 * Initialize the libevent library.
 */
static int
init_libevent(void)
{
  configure_libevent_logging();
  /* If the kernel complains that some method (say, epoll) doesn't
   * exist, we don't care about it, since libevent will cope.
   */
  suppress_libevent_log_msg("Function not implemented");
#ifdef __APPLE__
  setenv("EVENT_NOKQUEUE","1",1);
#endif
  event_init();
  suppress_libevent_log_msg(NULL);
#if defined(HAVE_EVENT_GET_VERSION) && defined(HAVE_EVENT_GET_METHOD)
  /* Making this a NOTICE for now so we can link bugs to a libevent versions
   * or methods better. */
  log(LOG_NOTICE, LD_GENERAL,
      "Initialized libevent version %s using method %s. Good.",
      event_get_version(), event_get_method());
  check_libevent_version(event_get_method(), event_get_version(),
                         get_options()->ORPort != 0);
#else
  log(LOG_NOTICE, LD_GENERAL,
      "Initialized old libevent (version 1.0b or earlier).");
  log(LOG_WARN, LD_GENERAL,
      "You have a very old version of libevent.  It is likely to be buggy; "
      "please consider building Tor with a more recent version.");
#endif

  return 0;
}

#if defined(HAVE_EVENT_GET_VERSION) && defined(HAVE_EVENT_GET_METHOD)
/**
 * Compare the given libevent method and version to a list of versions
 * which are known not to work.  Warn the user as appropriate.
 *
 */
static void
check_libevent_version(const char *m, const char *v, int server)
{
  int buggy = 0, iffy = 0, slow = 0;

  tor_assert(m && v);

  if (!strcmp(m, "kqueue")) {
    if (!strcmp(v, "1.0c") || !strcmp(v, "1.0d") || !strcmp(v, "1.0e") ||
        !strcmp(v, "1.1")) {
      buggy = 1;
    }
  } else if (!strcmp(m, "epoll")) {
    if (!strcmp(v, "1.0c") || !strcmp(v, "1.0d") || !strcmp(v, "1.0e"))
      iffy = 1;
  } else if (!strcmp(m, "poll")) {
    if (!strcmp(v, "1.0c") || !strcmp(v, "1.0d"))
      buggy = 1;
    else if (!strcmp(v, "1.0e"))
      slow = 1;
  } else if (!strcmp(m, "poll")) {
    if (!strcmp(v, "1.0c") || !strcmp(v, "1.0d") || !strcmp(v, "1.0e"))
      slow = 1;
  }

  if (buggy) {
    log(LOG_WARN, LD_GENERAL,
        "There are known bugs in using %s with libevent %s. "
        "Please use the latest version of libevent.", m, v);
  } else if (iffy) {
    log(LOG_WARN, LD_GENERAL,
        "There are minor bugs in using %s with libevent %s. "
        "You may want to use the latest version of libevent.", m, v);
  } else if (slow && server) {
    log(LOG_WARN, LD_GENERAL,
        "libevent %s can be very slow with %s. "
        "When running a server, please use the latest version of libevent.",
        v,m);
  }

}
#endif

/* Versioning issues and state: we want to be able to understand old state
 * files, and not choke on new ones.
 *
 * We could preserve all unrecognized variables across invocations, but we
 * could screw up order, if their order is significant with respect to
 * existing options.
 *
 * We could just dump unrecognized variables if you downgrade.
 *
 * This needs thought. XXXX NM
 */

/** DOCDOC */
or_state_t *
get_or_state(void)
{
  return global_state;
}

/** DOCDOC */
static char *
get_or_state_fname(void)
{
  char *fname = NULL;
  or_options_t *options = get_options();
  size_t len = strlen(options->DataDirectory) + 16;
  fname = tor_malloc(len);
  tor_snprintf(fname, len, "%s/state", options->DataDirectory);
  return fname;
}

/** DOCDOC */
/* XXX from_setconf is here because of bug 238 */
static int
or_state_validate(or_state_t *old_state, or_state_t *state, int from_setconf)
{
  const char *err;
  tor_version_t v;
  if (entry_guards_parse_state(state, 0, &err)<0) {
    warn(LD_GENERAL, "Unable to parse entry nodes: %s", err);
    return -1;
  }
  if (state->TorVersion && tor_version_parse(state->TorVersion, &v)) {
    warn(LD_GENERAL, "Can't parse Tor version '%s' from your state file. "
         "Proceeding anyway.", state->TorVersion);
  }
  return 0;
}

/** DOCDOC */
static void
or_state_set(or_state_t *new_state)
{
  const char *err;
  tor_assert(new_state);
  if (global_state)
    config_free(&state_format, global_state);
  global_state = new_state;
  if (entry_guards_parse_state(global_state, 1, &err)<0)
    warn(LD_GENERAL,"Unparseable helper nodes state: %s",err);
  if (rep_hist_load_state(global_state, &err)<0)
    warn(LD_GENERAL,"Unparseable bandwidth history state: %s",err);
}

/* DOCDOC */
int
or_state_load(void)
{
  or_state_t *new_state = NULL;
  char *contents = NULL, *fname;
  int r = -1;

  fname = get_or_state_fname();
  switch (file_status(fname)) {
    case FN_FILE:
      if (!(contents = read_file_to_str(fname, 0))) {
        warn(LD_FS, "Unable to read state file \"%s\"", fname);
        goto done;
      }
      break;
    case FN_NOENT:
      break;
    default:
      warn(LD_GENERAL,"State file \"%s\" is not a file? Failing.", fname);
      goto done;
  }
  new_state = tor_malloc_zero(sizeof(or_state_t));
  new_state->_magic = OR_STATE_MAGIC;
  config_init(&state_format, new_state);
  if (contents) {
    config_line_t *lines=NULL;
    int assign_retval;
    if (config_get_lines(contents, &lines)<0)
      goto done;
    assign_retval = config_assign(&state_format, new_state, lines, 0, 0);
    config_free_lines(lines);
    if (assign_retval<0)
      goto done;
  }

  if (or_state_validate(NULL, new_state, 1) < 0)
    goto done;

  if (contents)
    info(LD_GENERAL, "Loaded state from \"%s\"", fname);
  else
    info(LD_GENERAL, "Initialized state");
  or_state_set(new_state);
  new_state = NULL;
  if (!contents) {
    global_state->dirty = 1;
    or_state_save();
  }

  r = 0;
 done:
  tor_free(fname);
  tor_free(contents);
  if (new_state)
    config_free(&state_format, new_state);

  return r;
}

/** DOCDOC */
int
or_state_save(void)
{
  char *state, *contents;
  char tbuf[ISO_TIME_LEN+1];
  size_t len;
  char *fname;

  tor_assert(global_state);

  entry_guards_update_state(global_state);
  rep_hist_update_state(global_state);

  if (!global_state->dirty)
    return 0;

  global_state->LastWritten = time(NULL);
  tor_free(global_state->TorVersion);
  global_state->TorVersion = tor_strdup("Tor " VERSION);
  state = config_dump(&state_format, global_state, 0);
  len = strlen(state)+128;
  contents = tor_malloc(len);
  format_local_iso_time(tbuf, time(NULL));
  tor_snprintf(contents, len,
               "# Tor state file last generated on %s\n"
               "# You *do not* need to edit this file.\n\n%s",
               tbuf, state);
  tor_free(state);
  fname = get_or_state_fname();
  if (write_str_to_file(fname, contents, 0)<0) {
    warn(LD_FS, "Unable to write state to file \"%s\"", fname);
    tor_free(fname);
    tor_free(contents);
    return -1;
  }
  info(LD_GENERAL, "Saved state to \"%s\"", fname);
  tor_free(fname);
  tor_free(contents);

  global_state->dirty = 0;
  return 0;
}

/** DOCDOC */
int
config_getinfo_helper(const char *question, char **answer)
{
  if (!strcmp(question, "config/names")) {
    smartlist_t *sl = smartlist_create();
    int i;
    for (i = 0; _option_vars[i].name; ++i) {
      config_var_t *var = &_option_vars[i];
      const char *type, *desc;
      char *line;
      size_t len;
      desc = config_find_description(&options_format, var->name);
      switch (var->type) {
        case CONFIG_TYPE_STRING: type = "String"; break;
        case CONFIG_TYPE_UINT: type = "Integer"; break;
        case CONFIG_TYPE_INTERVAL: type = "TimeInterval"; break;
        case CONFIG_TYPE_MEMUNIT: type = "DataSize"; break;
        case CONFIG_TYPE_DOUBLE: type = "Float"; break;
        case CONFIG_TYPE_BOOL: type = "Boolean"; break;
        case CONFIG_TYPE_ISOTIME: type = "Time"; break;
        case CONFIG_TYPE_CSV: type = "CommaList"; break;
        case CONFIG_TYPE_LINELIST: type = "LineList"; break;
        case CONFIG_TYPE_LINELIST_S: type = "Dependant"; break;
        case CONFIG_TYPE_LINELIST_V: type = "Virtual"; break;
        default:
        case CONFIG_TYPE_OBSOLETE:
          type = NULL; break;
      }
      if (!type)
        continue;
      len = strlen(var->name)+strlen(type)+16;
      if (desc)
        len += strlen(desc);
      line = tor_malloc(len);
      if (desc)
        tor_snprintf(line, len, "%s %s %s\n",var->name,type,desc);
      else
        tor_snprintf(line, len, "%s %s\n",var->name,type);
      smartlist_add(sl, line);
    }
    *answer = smartlist_join_strings(sl, "", 0, NULL);
    SMARTLIST_FOREACH(sl, char *, c, tor_free(c));
    smartlist_free(sl);
  }
  return 0;
}

/** Dump the version of every file to the log. */
static void
print_cvs_version(void)
{
  extern const char aes_c_id[];
  extern const char compat_c_id[];
  extern const char container_c_id[];
  extern const char crypto_c_id[];
  extern const char log_c_id[];
  extern const char torgzip_c_id[];
  extern const char tortls_c_id[];
  extern const char util_c_id[];

  extern const char buffers_c_id[];
  extern const char circuitbuild_c_id[];
  extern const char circuitlist_c_id[];
  extern const char circuituse_c_id[];
  extern const char command_c_id[];
//  extern const char config_c_id[];
  extern const char connection_c_id[];
  extern const char connection_edge_c_id[];
  extern const char connection_or_c_id[];
  extern const char control_c_id[];
  extern const char cpuworker_c_id[];
  extern const char directory_c_id[];
  extern const char dirserv_c_id[];
  extern const char dns_c_id[];
  extern const char hibernate_c_id[];
  extern const char main_c_id[];
  extern const char onion_c_id[];
  extern const char relay_c_id[];
  extern const char rendclient_c_id[];
  extern const char rendcommon_c_id[];
  extern const char rendmid_c_id[];
  extern const char rendservice_c_id[];
  extern const char rephist_c_id[];
  extern const char router_c_id[];
  extern const char routerlist_c_id[];
  extern const char routerparse_c_id[];

  puts(AES_H_ID);
  puts(COMPAT_H_ID);
  puts(CONTAINER_H_ID);
  puts(CRYPTO_H_ID);
  puts(LOG_H_ID);
  puts(TORGZIP_H_ID);
  puts(TORINT_H_ID);
  puts(TORTLS_H_ID);
  puts(UTIL_H_ID);
  puts(aes_c_id);
  puts(compat_c_id);
  puts(container_c_id);
  puts(crypto_c_id);
  puts(log_c_id);
  puts(torgzip_c_id);
  puts(tortls_c_id);
  puts(util_c_id);

  puts(OR_H_ID);
  puts(buffers_c_id);
  puts(circuitbuild_c_id);
  puts(circuitlist_c_id);
  puts(circuituse_c_id);
  puts(command_c_id);
  puts(config_c_id);
  puts(connection_c_id);
  puts(connection_edge_c_id);
  puts(connection_or_c_id);
  puts(control_c_id);
  puts(cpuworker_c_id);
  puts(directory_c_id);
  puts(dirserv_c_id);
  puts(dns_c_id);
  puts(hibernate_c_id);
  puts(main_c_id);
  puts(onion_c_id);
  puts(relay_c_id);
  puts(rendclient_c_id);
  puts(rendcommon_c_id);
  puts(rendmid_c_id);
  puts(rendservice_c_id);
  puts(rephist_c_id);
  puts(router_c_id);
  puts(routerlist_c_id);
  puts(routerparse_c_id);
}

