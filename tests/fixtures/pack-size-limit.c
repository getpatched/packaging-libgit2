#include <git2.h>
#include <git2/indexer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_file(const char *path, char **data, size_t *length)
{
	FILE *stream;
	long size;

	if (!(stream = fopen(path, "rb")))
		return -1;
	if (fseek(stream, 0, SEEK_END) != 0 || (size = ftell(stream)) <= 0 ||
	    fseek(stream, 0, SEEK_SET) != 0) {
		fclose(stream);
		return -1;
	}
	*data = malloc((size_t)size);
	if (!*data || fread(*data, 1, (size_t)size, stream) != (size_t)size) {
		free(*data);
		fclose(stream);
		return -1;
	}
	*length = (size_t)size;
	fclose(stream);
	return 0;
}

int main(int argc, char **argv)
{
	git_indexer *indexer = NULL;
	git_indexer_options options = GIT_INDEXER_OPTIONS_INIT;
	git_transfer_progress progress = { 0 };
	char *pack = NULL;
	size_t pack_size = 0;
	int error;
#ifdef GETPATCHED_PATCHED
	int rejected_as_large = 0;
#endif

	if (argc != 3 || read_file(argv[1], &pack, &pack_size) < 0)
		return 2;
	if (git_libgit2_init() < 0)
		return 2;
#ifdef GETPATCHED_PATCHED
	if (git_libgit2_opts(GIT_OPT_SET_PACK_MAX_OBJECT_SIZE, 42 * 1024 * 1024) < 0)
		return 2;
#endif
	error = git_indexer_new(&indexer, argv[2], 0, NULL, &options);
	if (error == 0)
		error = git_indexer_append(indexer, pack, pack_size, &progress);
	if (error == 0)
		error = git_indexer_commit(indexer, &progress);
#ifdef GETPATCHED_PATCHED
	rejected_as_large = error < 0 && git_error_last() &&
		strstr(git_error_last()->message, "overly large object") != NULL;
#endif
	git_indexer_free(indexer);
	free(pack);
	git_libgit2_shutdown();
#ifdef GETPATCHED_PATCHED
	if (!rejected_as_large)
		return 1;
	puts("result=rejected:overly-large-object");
#else
	if (error < 0)
		return 1;
	puts("result=accepted:large-object");
#endif
	return 0;
}
