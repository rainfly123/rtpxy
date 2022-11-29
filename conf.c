 /*
  *
  * Copyright (C) 2022 Rainfly <18910158363>
  * 
  */
   
#include <stdlib.h>
#include <stdio.h>
  
#include <cyaml/cyaml.h>
#include "conf.h"
 
static const cyaml_schema_field_t channels_fields_schema[] = {
	/* The first field in the mapping is description.
	 *
	 * YAML key: "Description".
	 * C structure member for this key: "description".
	 *
	 * Its CYAML type is string pointer, and we have no minimum or maximum
	 * string length constraints.
	 */
	CYAML_FIELD_STRING_PTR(
			"type", CYAML_FLAG_POINTER,
			struct channels, type, 0, CYAML_UNLIMITED),
 
	CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER, struct channels, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("url",  CYAML_FLAG_POINTER, struct channels, url, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("sdev", CYAML_FLAG_POINTER, struct channels, sdev, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("dst",  CYAML_FLAG_POINTER, struct channels, dst, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("dev",  CYAML_FLAG_POINTER, struct channels, dev, 0, CYAML_UNLIMITED),
	/* The field array must be terminated by an entry with a NULL key.
	 * Here we use the CYAML_FIELD_END macro for that. */
	CYAML_FIELD_END
};
 
/* The value for a channels is a mapping.
 *
 * Its fields are defined in channels_fields_schema.
 */
static const cyaml_schema_value_t channels_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
			struct channels, channels_fields_schema),
};
 
 
/* The plan mapping's field definitions schema is an array.
 *
 * All the field entries will refer to their parent mapping's structure,
 * in this case, `struct plan`.
 */
static const cyaml_schema_field_t conf_fields_schema[] = {
 
	CYAML_FIELD_STRING_PTR(
			"key", CYAML_FLAG_POINTER,
			struct conf, key, 0, CYAML_UNLIMITED),
 
	CYAML_FIELD_SEQUENCE(
			"channels", CYAML_FLAG_POINTER,
			struct conf, chans,
			&channels_schema, 0, CYAML_UNLIMITED),
 
	/* The field array must be terminated by an entry with a NULL key.
	 * Here we use the CYAML_FIELD_END macro for that. */
	CYAML_FIELD_END
};
 
 
/* Top-level schema.  The top level value for the plan is a mapping.
 *
 * Its fields are defined in plan_fields_schema.
 */
static const cyaml_schema_value_t conf_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER,
			struct conf, conf_fields_schema),
};

static const cyaml_config_t config = {
    .log_fn = cyaml_log,            /* Use the default logging function. */
    .mem_fn = cyaml_mem,            /* Use the default memory allocator. */
    .log_level = CYAML_LOG_WARNING, /* Logging errors and warnings only. */
};

struct conf * parse_conf(void)
{
         cyaml_err_t err;
         struct conf *conf;
         /* Load input file. */
         err = cyaml_load_file("conf.yaml", &config,
                         &conf_schema, (void **) &conf, NULL);
         if (err != CYAML_OK) {
                 fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err));
                 return NULL;
         }
 
         /* Use the data. */
         #if 0
         printf("key: %s\n", conf->key);
         for (unsigned i = 0; i < conf->chans_count; i++) {
                 printf("%u. type: %s\n", i + 1, conf->chans[i].type);
                 printf("name: %s\n", conf->chans[i].name);
                 printf("url: %s\n", conf->chans[i].url);
                 printf("sdev: %s\n", conf->chans[i].sdev);
                 printf("dst: %s\n", conf->chans[i].dst);
                 printf("dev: %s\n", conf->chans[i].dev);
         }       
         #endif
         
         //cyaml_free(&config, &conf_schema, conf, 0);
         cyaml_free(&config, &conf_schema, NULL, 0);
         
         return conf;
}
