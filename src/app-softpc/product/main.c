#include "config.h"
#include "extensions.h"
#include "composed_machine.h"
#include "x86/product/entry_interface.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    const char *product_name = "Insignia SoftPC";
    char config_path[SOFTPC_CONFIG_PATH_MAX];
    app_startup_config config = { { 0 }, { 0 }, { 0 }, { 0 }, 16u * 1024u * 1024u,
        EMULATOR_SESSION_DISPLAY_CONSOLE, 1, LIB_STORAGE_MEDIUM_OVERLAY,
        LIB_STORAGE_MEDIUM_OVERLAY };
    x86_product_definition definition;
    app_composed_machine machine;
    app_composed_ui ui;
    (void)argv;

    if (argc != 1) {
        fprintf(stderr, "softpcvm: command-line arguments are not supported\n");
        return 2;
    }
    if (!app_get_config_path(config_path)) {
        fprintf(stderr, "softpcvm: cannot determine adjacent softpc.ini path\n");
        return 1;
    }
    if (!app_load_startup_config(config_path, &config)) {
        fprintf(stderr, "softpcvm: cannot read fixed-machine config '%s'\n",
            config_path);
        return 1;
    }
    if (!app_resolve_image_path(config.floppy_path, config_path) ||
        !app_resolve_image_path(config.hard_disk_path, config_path) ||
        !app_resolve_image_path(config.serial_output_path, config_path) ||
        !app_resolve_image_path(config.printer_output_path, config_path)) {
        fprintf(stderr, "softpcvm: path in '%s' is too long\n", config_path);
        return 1;
    }
    if (app_startup_compose_ui(&config, &ui) != LIB_STATUS_OK ||
        softpc_product_compose_machine(&config, &machine) != LIB_STATUS_OK) {
        fprintf(stderr, "softpcvm: cannot compose machine\n");
        return 1;
    }
    definition = (x86_product_definition){
        .name = product_name,
        .banner = product_name,
        .machine = machine,
        .ui = ui,
        .configure_extensions = softpc_product_configure_extensions
    };
    return x86_product_run(&definition);
}
