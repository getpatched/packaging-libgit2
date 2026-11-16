#include <assert.h>
#include <git2.h>
#include <string.h>

int main(int argc, char **argv) {
    const char content[] = "Installed libgit2 object storage\n";
    git_repository *repo = NULL;
    git_odb *odb = NULL;
    git_blob *blob = NULL;
    git_oid id;

    assert(argc == 2);
    assert(git_libgit2_init() > 0);
    assert(git_repository_init(&repo, argv[1], 1) == 0);
    assert(git_repository_odb(&odb, repo) == 0);
    assert(git_odb_write(&id, odb, content, sizeof(content) - 1, GIT_OBJECT_BLOB) == 0);
    assert(git_blob_lookup(&blob, repo, &id) == 0);
    assert(git_blob_rawsize(blob) == sizeof(content) - 1);
    assert(memcmp(git_blob_rawcontent(blob), content, sizeof(content) - 1) == 0);

    git_blob_free(blob);
    git_odb_free(odb);
    git_repository_free(repo);
    git_libgit2_shutdown();
    return 0;
}
