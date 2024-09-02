#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webview/webview.h>

#include "resources.h"
#include "utils.h"

struct resources
{
	char *html_contents;
	char *processed_template;
};

typedef struct resources resources_t;

static resources_t *resources_init(void)
{
	resources_t *r;

	r = alloc(sizeof(resources_t));

	r->html_contents = resource_read("index.html");
	r->processed_template = resource_template_process(r->html_contents);

	return r;
}

static void resources_load(resources_t *r, webview_t *w)
{
	assert(strcmp(r->processed_template, "") != 0);
	puts(r->processed_template);
	webview_set_html(*w, r->processed_template);
}

static void resources_deinit(resources_t *r)
{
	free(r->html_contents);
	free(r->processed_template);
	free(r);
}

static void display_impl(const char *id, const char *req, void *arg) { printf("received from client -> %s\n", req); }

static void send_notification_impl(const char *id, const char *req, void *arg)
{
	/* req comes like ["hello"] so we're gonna ignore the special symbols to obtain the first element */
	char param_buf[strlen(req) - 3], command_buf[100];
	int i, j;

	for (i = j = 0; req[i] != '\0'; ++i)
		if (req[i] != '[' && req[i] != '"' && req[i] != ']')
			param_buf[j++] = req[i];

	param_buf[j] = '\0';

	/* we're gonna spawn a notify-send process so one needs to have libnotify installed */
	strcpy(command_buf, "notify-send '");
	strncat(command_buf, param_buf, strlen(param_buf) + 1);
	strcat(command_buf, "'");

	system(command_buf);

	/* cleaning up */
	memset(param_buf, 0, sizeof(param_buf));
	memset(command_buf, 0, sizeof(command_buf));
}

static void load_bindings(webview_t *w)
{
	webview_bind(*w, "display", display_impl, NULL);
	webview_bind(*w, "sendNotification", send_notification_impl, NULL);
}

int main(int argc, char *argv[])
{
	webview_t w;
	resources_t *r;

	r = resources_init();
	w = webview_create(0, NULL);

	webview_set_title(w, "Window");
	webview_set_size(w, 800, 600, WEBVIEW_HINT_NONE);
	load_bindings(&w);
	resources_load(r, &w);
	webview_run(w);
	webview_destroy(w);

	resources_deinit(r);

	return 0;
}
