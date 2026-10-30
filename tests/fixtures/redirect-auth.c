#include <git2.h>
#include <git2/clone.h>

#include <stdio.h>
#include <string.h>

struct callback_state {
	const char *expected_origin;
	int called;
	int matched;
};

static int capture_credentials(
	git_cred **out,
	const char *url,
	const char *username,
	unsigned int allowed_types,
	void *payload)
{
	struct callback_state *state = payload;
	size_t expected_length = strlen(state->expected_origin);

	(void)out;
	(void)username;
	(void)allowed_types;
	state->called = 1;
	state->matched = strncmp(url, state->expected_origin, expected_length) == 0 &&
		(url[expected_length] == '/' || url[expected_length] == '\0');
	return GIT_PASSTHROUGH;
}

int main(int argc, char **argv)
{
	git_clone_options options = GIT_CLONE_OPTIONS_INIT;
	git_repository *repo = NULL;
	struct callback_state state;
	int error;

	if (argc != 4 || git_libgit2_init() < 0)
		return 2;
	state.expected_origin = argv[2];
	state.called = 0;
	state.matched = 0;
	options.fetch_opts.callbacks.credentials = capture_credentials;
	options.fetch_opts.callbacks.payload = &state;
	error = git_clone(&repo, argv[1], argv[3], &options);
	git_repository_free(repo);
	git_libgit2_shutdown();
	if (error >= 0 || !state.called || !state.matched) {
		fprintf(stderr, "redirect credential callback did not receive the redirected origin\n");
		return 1;
	}
	return 0;
}
