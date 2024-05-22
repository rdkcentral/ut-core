#include <libwebsockets.h>
#include <stdio.h>

char *test_message="HI FROM SERVER";

static int callback_echo(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
        printf("Client connected\n");
        break;

    case LWS_CALLBACK_RECEIVE:
        printf("LWS_CALLBACK_RECEIVE\n");
        // Echo back received message
        printf("Received message: %.*s\n", (int)len, (char *)in);
        lws_write(wsi, in, len, LWS_WRITE_TEXT);
        break;

    default:
        break;
    }

    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "echo-protocol",    // protocol name
        callback_echo,      // callback function
        0,                  // per-session data size
        0,                  // rx buffer size
        0,                  // id
        NULL,               // reserved
        0                   // tx packet size
    },
    { NULL, NULL, 0, 0 }    // end of list
};

void ut_control_panel()
{
    struct lws_context_creation_info info;
    struct lws_context *context;
    const char *address = "localhost";
    int port = 8080;

    memset(&info, 0, sizeof(info));
    info.port = port;
    info.iface = NULL;
    info.protocols = protocols;

    context = lws_create_context(&info);
    if (!context)
    {
        fprintf(stderr, "Error creating libwebsockets context\n");
        return;
    }

    printf("WebSocket server started at %s:%d\n", address, port);

    while (1)
    {
        lws_service(context, 1000);
    }

    lws_context_destroy(context);

    return;
}