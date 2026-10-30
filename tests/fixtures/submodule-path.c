#include <git2.h>
#include <git2/submodule.h>

#include <stdio.h>

int main(int argc, char **argv)
{
	git_repository *repo = NULL, *subrepo = NULL;
	git_submodule *submodule = NULL;
	FILE *config;
	char config_path[4096];
	int error, written;

	if (argc != 2 || git_libgit2_init() < 0)
		return 2;
	if (git_repository_init(&repo, argv[1], 0) < 0)
		return 2;
	written = snprintf(config_path, sizeof(config_path), "%s/.gitmodules", git_repository_workdir(repo));
	if (written < 0 || (size_t)written >= sizeof(config_path) ||
	    !(config = fopen(config_path, "w"))) {
		git_repository_free(repo);
		return 2;
	}
	fputs("[submodule \"escape\"]\n"
	      "    path = ../escape-target\n"
	      "    url = file:///unused\n", config);
	fclose(config);

	error = git_submodule_lookup(&submodule, repo, "escape");
#ifdef GETPATCHED_PATCHED
	if (error != GIT_EINVALID) {
		git_submodule_free(submodule);
		git_repository_free(repo);
		return 1;
	}
	puts("result=rejected:invalid-submodule-path");
#else
	if (error < 0 || git_submodule_init(submodule, 0) < 0 ||
	    git_submodule_repo_init(&subrepo, submodule, 1) < 0) {
		git_submodule_free(submodule);
		git_repository_free(repo);
		return 1;
	}
	puts("result=created:escaped-submodule-directory");
	git_repository_free(subrepo);
#endif
	git_submodule_free(submodule);
	git_repository_free(repo);
	git_libgit2_shutdown();
	return 0;
}
