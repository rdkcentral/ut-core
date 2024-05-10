#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libwebsockets.h>

#define WEBSOCKET_SERVER_ADDRESS "10.242.27.138"

static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {

    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("Connection established\n");
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            // Message received - assuming JSON format like {"symbol": "AAPL", "price": 155.23}
            printf("Received message: %.*s\n", (int)len, (char *)in);  
            // TODO: Parse the JSON for symbol and price
            break;

        // Other events as needed ... (connection closed, etc.)

        default:
            break;
    }

    return 0;
}

static struct lws_protocols protocols[] = {
    { "http-only", callback_http, 0, 0 },
    { NULL, NULL, 0, 0 } /* terminator */
};

int ut_control_panel(void) {
    struct lws_context_creation_info info;
    struct lws_context *context;
    const char *server_address = WEBSOCKET_SERVER_ADDRESS;
    int port = 80; // Or whatever port is used by your provider

    memset(&info, 0, sizeof info);
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;

    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "Failed to create libwebsockets context\n");
        return 1;
    }
#if 0
    // Connect to the server
    struct lws *wsi = lws_client_connect (context, server_address, port, 0, "/", server_address, NULL, NULL, -1);
    if (!wsi) {
        fprintf(stderr, "Failed to connect to server\n");
        lws_context_destroy(context);
        return 1;
    }
#endif

    // Event loop
    while (1) {
        lws_service(context, 1000); // Timeout of 1000 ms
    }

    lws_context_destroy(context);
    return 0;
}
