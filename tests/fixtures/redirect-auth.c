#include <git2.h>
#include <git2/clone.h>

#include <stdio.h>
struct callback_state {
	int called;
};

static int capture_credentials(
	git_cred **out,
	const char *url,
	const char *username,
	unsigned int allowed_types,
	void *payload)
{
	struct callback_state *state = payload;
	(void)out;
	(void)username;
	(void)allowed_types;
	state->called = 1;
	printf("callback_url=%s\n", url);
	return GIT_PASSTHROUGH;
}

int main(int argc, char **argv)
{
	git_clone_options options = GIT_CLONE_OPTIONS_INIT;
	git_repository *repo = NULL;
	struct callback_state state;
	int error;

	if (argc != 3 || git_libgit2_init() < 0)
		return 2;
	state.called = 0;
	options.fetch_opts.callbacks.credentials = capture_credentials;
	options.fetch_opts.callbacks.payload = &state;
	error = git_clone(&repo, argv[1], argv[2], &options);
	git_repository_free(repo);
	git_libgit2_shutdown();
	if (error >= 0 || !state.called) {
		fprintf(stderr, "redirect credential callback was not invoked as expected\n");
		return 1;
	}
	return 0;
}
