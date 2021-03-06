#include "config.h"
#include <stdio.h>
#include <check.h>
#include <stdlib.h>
#include "r3.h"
#include "r3_str.h"
#include "str_array.h"
#include "bench.h"



START_TEST (test_ltrim_slash)
{
    fail_if( strcmp( ltrim_slash("/blog") , "blog" ) != 0 );
    fail_if( strcmp( ltrim_slash("blog") , "blog" ) != 0 );
}
END_TEST

START_TEST (test_r3_node_construct_uniq)
{
    node * n = r3_tree_create(10);

    node * child = r3_tree_create(3);

    // fail_if( r3_node_add_child(n, strdup("/add") , child) != NULL );
    // fail_if( r3_node_add_child(n, strdup("/add") , child) != NULL );

    r3_tree_free(n);
}
END_TEST

START_TEST (test_r3_node_find_edge)
{
    node * n = r3_tree_create(10);

    node * child = r3_tree_create(3);

    fail_if( r3_node_add_child(n, strdup("/add") , child) == FALSE );

    fail_if( r3_node_find_edge(n, "/add") == NULL );
    fail_if( r3_node_find_edge(n, "/bar") != NULL );

    r3_tree_free(n);
}
END_TEST


START_TEST (test_compile)
{
    str_array *t;
    node * n;
    n = r3_tree_create(10);


    node *m;
    edge *e ;

    r3_tree_insert_pathl(n, "/zoo", strlen("/zoo"), NULL, NULL);
    r3_tree_insert_pathl(n, "/foo", strlen("/foo"), NULL, NULL);
    r3_tree_insert_pathl(n, "/bar", strlen("/bar"), NULL, NULL);
    r3_tree_compile(n);
    fail_if( n->combined_pattern );
    fail_if( NULL == r3_node_find_edge_str(n, "/", strlen("/") ) );

#ifdef DEBUG
    r3_tree_dump(n, 0);
#endif

    r3_tree_insert_pathl(n, "/foo/{id}", strlen("/foo/{id}"), NULL, NULL);
    r3_tree_insert_pathl(n, "/{id}", strlen("/{id}"), NULL, NULL);
    r3_tree_compile(n);
    r3_tree_compile(n); // test double compile
#ifdef DEBUG
    r3_tree_dump(n, 0);
#endif
    /*
    fail_if(n->edges[0]->child->combined_pattern == NULL);

    e = r3_node_find_edge_str(n, "/", strlen("/") );
    fail_if( NULL == e );
    */
    /*
    printf( "%s\n", e->pattern );
    printf( "%s\n", e->child->combined_pattern );
    printf( "%s\n", n->edges[0]->child->combined_pattern);
    printf( "%s\n", n->combined_pattern );
    */

    match_entry * entry;

    entry = match_entry_createl( "foo" , strlen("/foo") );
    m = r3_tree_match( n , "/foo", strlen("/foo"), entry);
    fail_if( NULL == m );

    entry = match_entry_createl( "/zoo" , strlen("/zoo") );
    m = r3_tree_match( n , "/zoo", strlen("/zoo"), entry);
    fail_if( NULL == m );

    entry = match_entry_createl( "/bar" , strlen("/bar") );
    m = r3_tree_match( n , "/bar", strlen("/bar"), entry);
    fail_if( NULL == m );

    entry = match_entry_createl( "/xxx" , strlen("/xxx") );
    m = r3_tree_match( n , "/xxx", strlen("/xxx"), entry);
    fail_if( NULL == m );

    entry = match_entry_createl( "/foo/xxx" , strlen("/foo/xxx") );
    m = r3_tree_match( n , "/foo/xxx", strlen("/foo/xxx"), entry);
    fail_if( NULL == m );

    entry = match_entry_createl( "/some_id" , strlen("/some_id") );
    m = r3_tree_match( n , "/some_id", strlen("/some_id"), entry);
    fail_if( NULL == m );
    ck_assert_int_gt( m->endpoint , 0 ); // should not be an endpoint
}
END_TEST

START_TEST (test_compile_slug)
{
    /*
    char * pattern;
    pattern = compile_slug("{id}", strlen("{id}"));
    ck_assert_str_eq( pattern, "([^/]+)" );
    free(pattern);

    pattern = compile_slug("/{id}", strlen("/{id}"));
    ck_assert_str_eq( pattern, "/([^/]+)" );
    free(pattern);

    pattern = compile_slug("-{id}", strlen("-{id}"));
    ck_assert_str_eq( pattern, "-([^-]+)" );
    free(pattern);

    pattern = compile_slug("{id}-{title}", strlen("{id}-{title}"));
    ck_assert_str_eq( pattern, "([^/]+)-([^-]+)" );
    free(pattern);


    pattern = compile_slug("{id:[a-z]+}", strlen("{id:[a-z]+}") );
    ck_assert_str_eq( pattern, "([a-z]+)" );
    free(pattern);


    pattern = compile_slug("/path/{id:[a-z]+}", strlen("/path/{id:[a-z]+}") );
    ck_assert_str_eq( pattern, "/path/([a-z]+)" );
    free(pattern);
    */

    /*
    char * p = malloc(sizeof(char) * 10);
    strncat(p, "foo", 3);
    free(p);
    */





}
END_TEST


START_TEST (test_r3_tree_insert_pathl)
{
    node * n = r3_tree_create(10);

    info("Inserting /foo/bar\n");
    r3_tree_insert_path(n, "/foo/bar", NULL, NULL);
    // r3_tree_dump(n, 0);

    info("Inserting /foo/zoo\n");
    r3_tree_insert_path(n, "/foo/zoo", NULL, NULL);
    // r3_tree_dump(n, 0);

    info("Inserting /f/id\n");
    r3_tree_insert_path(n, "/f/id" , NULL, NULL);
    // r3_tree_dump(n, 0);

    info("Inserting /post/{id}\n");
    r3_tree_insert_pathl(n, "/post/{id}", strlen("/post/{id}"), NULL, NULL);
    // r3_tree_dump(n, 0);

    info("Inserting /post/{handle}\n");
    r3_tree_insert_pathl(n, "/post/{handle}", strlen("/post/{handle}"), NULL, NULL);
    // r3_tree_dump(n, 0);

    info("Inserting /post/{handle}-{id}\n");
    r3_tree_insert_pathl(n, "/post/{handle}-{id}", strlen("/post/{handle}-{id}"), NULL, NULL);
    r3_tree_compile(n);

#ifdef DEBUG
    r3_tree_dump(n, 0);
#endif
    r3_tree_free(n);
}
END_TEST





START_TEST (test_str_array)
{
    str_array * l = str_array_create(3);
    fail_if( l == NULL );

    fail_if( FALSE == str_array_append(l, strdup("abc") ) );
    fail_if( l->len != 1 );

    fail_if( FALSE == str_array_append(l, strdup("foo") ) );
    fail_if( l->len != 2 );

    fail_if( FALSE == str_array_append(l, strdup("bar") ) );
    fail_if( l->len != 3 );

    fail_if( FALSE == str_array_append(l, strdup("zoo") ) );
    fail_if( l->len != 4 );

    fail_if( FALSE == str_array_resize(l, l->cap * 2) );

    str_array_free(l);
}
END_TEST

START_TEST(test_route_cmp)
{
    route *r1 = route_create("/blog/post");
    match_entry * m = match_entry_create("/blog/post");

    fail_if( route_cmp(r1, m) == -1, "should match");

    r1->request_method = METHOD_GET;
    m->request_method = METHOD_GET;
    fail_if( route_cmp(r1, m) == -1, "should match");

    r1->request_method = METHOD_GET;
    m->request_method = METHOD_POST;
    fail_if( route_cmp(r1, m) == 0, "should be different");

    r1->request_method = METHOD_GET;
    m->request_method = METHOD_POST | METHOD_GET;
    fail_if( route_cmp(r1, m) == -1, "should match");

    route_free(r1);
    match_entry_free(m);
}
END_TEST


START_TEST(test_pcre_pattern_simple)
{
    match_entry * entry;
    entry = match_entry_createl( "/user/123" , strlen("/user/123") );
    node * n = r3_tree_create(10);
    r3_tree_insert_pathl(n, "/user/{id:\\d+}", strlen("/user/{id:\\d+}"), NULL, NULL);
    r3_tree_insert_pathl(n, "/user", strlen("/user"), NULL, NULL);
    r3_tree_compile(n);
    r3_tree_dump(n, 0);
    node *matched;
    matched = r3_tree_match(n, "/user/123", strlen("/user/123"), entry);
    fail_if(matched == NULL);
    ck_assert_int_gt(entry->vars->len, 0);
    ck_assert_str_eq(entry->vars->tokens[0],"123");
}
END_TEST


START_TEST(test_pcre_pattern_more)
{
    match_entry * entry;
    entry = match_entry_createl( "/user/123" , strlen("/user/123") );
    node * n = r3_tree_create(10);

    int var0 = 5;
    int var1 = 100;
    int var2 = 200;
    int var3 = 300;

    info("var0: %p\n", &var0);
    info("var1: %p\n", &var1);
    info("var2: %p\n", &var2);
    info("var3: %p\n", &var3);

    r3_tree_insert_pathl(n, "/user/{id:\\d+}", strlen("/user/{id:\\d+}"), NULL, &var1);
    r3_tree_insert_pathl(n, "/user2/{id:\\d+}", strlen("/user2/{id:\\d+}"), NULL, &var2);
    r3_tree_insert_pathl(n, "/user3/{id:\\d{3}}", strlen("/user3/{id:\\d{3}}"), NULL, &var3);
    r3_tree_insert_pathl(n, "/user", strlen("/user"), NULL, &var0);
    r3_tree_compile(n);
    r3_tree_dump(n, 0);
    node *matched;

    matched = r3_tree_match(n, "/user/123", strlen("/user/123"), entry);
    fail_if(matched == NULL);
    ck_assert_int_gt(entry->vars->len, 0);
    ck_assert_str_eq(entry->vars->tokens[0],"123");

    info("matched %p\n", matched->data);
    info("matched %p\n", matched->data);
    ck_assert_int_eq( *((int*) matched->data), var1);

    matched = r3_tree_match(n, "/user2/123", strlen("/user2/123"), entry);
    fail_if(matched == NULL);
    ck_assert_int_gt(entry->vars->len, 0);
    ck_assert_str_eq(entry->vars->tokens[0],"123");
    ck_assert_int_eq( *((int*)matched->data), var2);

    matched = r3_tree_match(n, "/user3/123", strlen("/user3/123"), entry);
    fail_if(matched == NULL);
    ck_assert_int_gt(entry->vars->len, 0);
    ck_assert_str_eq(entry->vars->tokens[0],"123");
    ck_assert_int_eq( *((int*)matched->data), var3);
}
END_TEST




START_TEST(test_insert_route)
{
    int   var1 = 22;
    int   var2 = 33;
    route *r1 = route_create("/blog/post");
    route *r2 = route_create("/blog/post");
    r1->request_method = METHOD_GET;
    r2->request_method = METHOD_POST;

    match_entry * entry = match_entry_create("/blog/post");
    entry->request_method = METHOD_GET;

    node * n = r3_tree_create(2);
    r3_tree_insert_route(n, r1, &var1);
    r3_tree_insert_route(n, r2, &var2);

    node *m;
    m = r3_tree_match(n , "/blog/post", strlen("/blog/post"), entry);

    fail_if(m == NULL);
    fail_if(m->endpoint == 0);
    route *c = r3_node_match_route(m, entry);
    fail_if(c == NULL);


    match_entry_free(entry);
    route_free(r1);
    route_free(r2);
}
END_TEST



START_TEST(benchmark_str)
{
    match_entry * entry = match_entry_createl("/blog/post", strlen("/blog/post") );
    node * n = r3_tree_create(1);


    int route_data = 999;

r3_tree_insert_path(n, "/foo/bar/baz", NULL, NULL);
r3_tree_insert_path(n, "/foo/bar/qux", NULL, NULL);
r3_tree_insert_path(n, "/foo/bar/quux", NULL, NULL);
r3_tree_insert_path(n, "/foo/bar/corge", NULL, NULL);
r3_tree_insert_path(n, "/foo/bar/grault", NULL, NULL);
r3_tree_insert_path(n, "/foo/bar/garply", NULL, NULL);
r3_tree_insert_path(n, "/foo/baz/bar", NULL, NULL);
r3_tree_insert_path(n, "/foo/baz/qux", NULL, NULL);
r3_tree_insert_path(n, "/foo/baz/quux", NULL, NULL);
r3_tree_insert_path(n, "/foo/baz/corge", NULL, NULL);
r3_tree_insert_path(n, "/foo/baz/grault", NULL, NULL);
r3_tree_insert_path(n, "/foo/baz/garply", NULL, NULL);
r3_tree_insert_path(n, "/foo/qux/bar", NULL, NULL);
r3_tree_insert_path(n, "/foo/qux/baz", NULL, NULL);
r3_tree_insert_path(n, "/foo/qux/quux", NULL, NULL);
r3_tree_insert_path(n, "/foo/qux/corge", NULL, NULL);
r3_tree_insert_path(n, "/foo/qux/grault", NULL, NULL);
r3_tree_insert_path(n, "/foo/qux/garply", NULL, NULL);
r3_tree_insert_path(n, "/foo/quux/bar", NULL, NULL);
r3_tree_insert_path(n, "/foo/quux/baz", NULL, NULL);
r3_tree_insert_path(n, "/foo/quux/qux", NULL, NULL);
r3_tree_insert_path(n, "/foo/quux/corge", NULL, NULL);
r3_tree_insert_path(n, "/foo/quux/grault", NULL, NULL);
r3_tree_insert_path(n, "/foo/quux/garply", NULL, NULL);
r3_tree_insert_path(n, "/foo/corge/bar", NULL, NULL);
r3_tree_insert_path(n, "/foo/corge/baz", NULL, NULL);
r3_tree_insert_path(n, "/foo/corge/qux", NULL, NULL);
r3_tree_insert_path(n, "/foo/corge/quux", NULL, NULL);
r3_tree_insert_path(n, "/foo/corge/grault", NULL, NULL);
r3_tree_insert_path(n, "/foo/corge/garply", NULL, NULL);
r3_tree_insert_path(n, "/foo/grault/bar", NULL, NULL);
r3_tree_insert_path(n, "/foo/grault/baz", NULL, NULL);
r3_tree_insert_path(n, "/foo/grault/qux", NULL, NULL);
r3_tree_insert_path(n, "/foo/grault/quux", NULL, NULL);
r3_tree_insert_path(n, "/foo/grault/corge", NULL, NULL);
r3_tree_insert_path(n, "/foo/grault/garply", NULL, NULL);
r3_tree_insert_path(n, "/foo/garply/bar", NULL, NULL);
r3_tree_insert_path(n, "/foo/garply/baz", NULL, NULL);
r3_tree_insert_path(n, "/foo/garply/qux", NULL, NULL);
r3_tree_insert_path(n, "/foo/garply/quux", NULL, NULL);
r3_tree_insert_path(n, "/foo/garply/corge", NULL, NULL);
r3_tree_insert_path(n, "/foo/garply/grault", NULL, NULL);
r3_tree_insert_path(n, "/bar/foo/baz", NULL, NULL);
r3_tree_insert_path(n, "/bar/foo/qux", NULL, NULL);
r3_tree_insert_path(n, "/bar/foo/quux", NULL, NULL);
r3_tree_insert_path(n, "/bar/foo/corge", NULL, NULL);
r3_tree_insert_path(n, "/bar/foo/grault", NULL, NULL);
r3_tree_insert_path(n, "/bar/foo/garply", NULL, NULL);
r3_tree_insert_path(n, "/bar/baz/foo", NULL, NULL);
r3_tree_insert_path(n, "/bar/baz/qux", NULL, NULL);
r3_tree_insert_path(n, "/bar/baz/quux", NULL, NULL);
r3_tree_insert_path(n, "/bar/baz/corge", NULL, NULL);
r3_tree_insert_path(n, "/bar/baz/grault", NULL, NULL);
r3_tree_insert_path(n, "/bar/baz/garply", NULL, NULL);
r3_tree_insert_path(n, "/bar/qux/foo", NULL, NULL);
r3_tree_insert_path(n, "/bar/qux/baz", NULL, NULL);
r3_tree_insert_path(n, "/bar/qux/quux", NULL, NULL);
r3_tree_insert_path(n, "/bar/qux/corge", NULL, NULL);
r3_tree_insert_path(n, "/bar/qux/grault", NULL, NULL);
r3_tree_insert_path(n, "/bar/qux/garply", NULL, NULL);
r3_tree_insert_path(n, "/bar/quux/foo", NULL, NULL);
r3_tree_insert_path(n, "/bar/quux/baz", NULL, NULL);
r3_tree_insert_path(n, "/bar/quux/qux", NULL, NULL);
r3_tree_insert_path(n, "/bar/quux/corge", NULL, NULL);
r3_tree_insert_path(n, "/bar/quux/grault", NULL, NULL);
r3_tree_insert_path(n, "/bar/quux/garply", NULL, NULL);
r3_tree_insert_path(n, "/bar/corge/foo", NULL, NULL);
r3_tree_insert_path(n, "/bar/corge/baz", NULL, NULL);
r3_tree_insert_path(n, "/bar/corge/qux", NULL, NULL);
r3_tree_insert_path(n, "/bar/corge/quux", NULL, NULL);
r3_tree_insert_path(n, "/bar/corge/grault", NULL, NULL);
r3_tree_insert_path(n, "/bar/corge/garply", NULL, NULL);
r3_tree_insert_path(n, "/bar/grault/foo", NULL, NULL);
r3_tree_insert_path(n, "/bar/grault/baz", NULL, NULL);
r3_tree_insert_path(n, "/bar/grault/qux", NULL, NULL);
r3_tree_insert_path(n, "/bar/grault/quux", NULL, NULL);
r3_tree_insert_path(n, "/bar/grault/corge", NULL, NULL);
r3_tree_insert_path(n, "/bar/grault/garply", NULL, NULL);
r3_tree_insert_path(n, "/bar/garply/foo", NULL, NULL);
r3_tree_insert_path(n, "/bar/garply/baz", NULL, NULL);
r3_tree_insert_path(n, "/bar/garply/qux", NULL, NULL);
r3_tree_insert_path(n, "/bar/garply/quux", NULL, NULL);
r3_tree_insert_path(n, "/bar/garply/corge", NULL, NULL);
r3_tree_insert_path(n, "/bar/garply/grault", NULL, NULL);
r3_tree_insert_path(n, "/baz/foo/bar", NULL, NULL);
r3_tree_insert_path(n, "/baz/foo/qux", NULL, NULL);
r3_tree_insert_path(n, "/baz/foo/quux", NULL, NULL);
r3_tree_insert_path(n, "/baz/foo/corge", NULL, NULL);
r3_tree_insert_path(n, "/baz/foo/grault", NULL, NULL);
r3_tree_insert_path(n, "/baz/foo/garply", NULL, NULL);
r3_tree_insert_path(n, "/baz/bar/foo", NULL, NULL);
r3_tree_insert_path(n, "/baz/bar/qux", NULL, NULL);
r3_tree_insert_path(n, "/baz/bar/quux", NULL, NULL);
r3_tree_insert_path(n, "/baz/bar/corge", NULL, NULL);
r3_tree_insert_path(n, "/baz/bar/grault", NULL, NULL);
r3_tree_insert_path(n, "/baz/bar/garply", NULL, NULL);
r3_tree_insert_path(n, "/baz/qux/foo", NULL, NULL);
r3_tree_insert_path(n, "/baz/qux/bar", NULL, NULL);
r3_tree_insert_path(n, "/baz/qux/quux", NULL, NULL);
r3_tree_insert_path(n, "/baz/qux/corge", NULL, NULL);
r3_tree_insert_path(n, "/baz/qux/grault", NULL, NULL);
r3_tree_insert_path(n, "/baz/qux/garply", NULL, NULL);
r3_tree_insert_path(n, "/baz/quux/foo", NULL, NULL);
r3_tree_insert_path(n, "/baz/quux/bar", NULL, NULL);
r3_tree_insert_path(n, "/baz/quux/qux", NULL, NULL);
r3_tree_insert_path(n, "/baz/quux/corge", NULL, NULL);
r3_tree_insert_path(n, "/baz/quux/grault", NULL, NULL);
r3_tree_insert_path(n, "/baz/quux/garply", NULL, NULL);
r3_tree_insert_path(n, "/baz/corge/foo", NULL, NULL);
r3_tree_insert_path(n, "/baz/corge/bar", NULL, NULL);
r3_tree_insert_path(n, "/baz/corge/qux", NULL, NULL);
r3_tree_insert_path(n, "/baz/corge/quux", NULL, NULL);
r3_tree_insert_path(n, "/baz/corge/grault", NULL, NULL);
r3_tree_insert_path(n, "/baz/corge/garply", NULL, NULL);
r3_tree_insert_path(n, "/baz/grault/foo", NULL, NULL);
r3_tree_insert_path(n, "/baz/grault/bar", NULL, NULL);
r3_tree_insert_path(n, "/baz/grault/qux", NULL, NULL);
r3_tree_insert_path(n, "/baz/grault/quux", NULL, NULL);
r3_tree_insert_path(n, "/baz/grault/corge", NULL, NULL);
r3_tree_insert_path(n, "/baz/grault/garply", NULL, NULL);
r3_tree_insert_path(n, "/baz/garply/foo", NULL, NULL);
r3_tree_insert_path(n, "/baz/garply/bar", NULL, NULL);
r3_tree_insert_path(n, "/baz/garply/qux", NULL, NULL);
r3_tree_insert_path(n, "/baz/garply/quux", NULL, NULL);
r3_tree_insert_path(n, "/baz/garply/corge", NULL, NULL);
r3_tree_insert_path(n, "/baz/garply/grault", NULL, NULL);
r3_tree_insert_path(n, "/qux/foo/bar", NULL, NULL);
r3_tree_insert_path(n, "/qux/foo/baz", NULL, NULL);
r3_tree_insert_path(n, "/qux/foo/quux", NULL, NULL);
r3_tree_insert_path(n, "/qux/foo/corge", NULL, NULL);
r3_tree_insert_path(n, "/qux/foo/grault", NULL, NULL);
r3_tree_insert_path(n, "/qux/foo/garply", NULL, NULL);
r3_tree_insert_path(n, "/qux/bar/foo", NULL, NULL);
r3_tree_insert_path(n, "/qux/bar/baz", NULL, NULL);
r3_tree_insert_path(n, "/qux/bar/quux", NULL, NULL);
r3_tree_insert_path(n, "/qux/bar/corge", NULL, &route_data);
r3_tree_insert_path(n, "/qux/bar/grault", NULL, NULL);
r3_tree_insert_path(n, "/qux/bar/garply", NULL, NULL);
r3_tree_insert_path(n, "/qux/baz/foo", NULL, NULL);
r3_tree_insert_path(n, "/qux/baz/bar", NULL, NULL);
r3_tree_insert_path(n, "/qux/baz/quux", NULL, NULL);
r3_tree_insert_path(n, "/qux/baz/corge", NULL, NULL);
r3_tree_insert_path(n, "/qux/baz/grault", NULL, NULL);
r3_tree_insert_path(n, "/qux/baz/garply", NULL, NULL);
r3_tree_insert_path(n, "/qux/quux/foo", NULL, NULL);
r3_tree_insert_path(n, "/qux/quux/bar", NULL, NULL);
r3_tree_insert_path(n, "/qux/quux/baz", NULL, NULL);
r3_tree_insert_path(n, "/qux/quux/corge", NULL, NULL);
r3_tree_insert_path(n, "/qux/quux/grault", NULL, NULL);
r3_tree_insert_path(n, "/qux/quux/garply", NULL, NULL);
r3_tree_insert_path(n, "/qux/corge/foo", NULL, NULL);
r3_tree_insert_path(n, "/qux/corge/bar", NULL, NULL);
r3_tree_insert_path(n, "/qux/corge/baz", NULL, NULL);
r3_tree_insert_path(n, "/qux/corge/quux", NULL, NULL);
r3_tree_insert_path(n, "/qux/corge/grault", NULL, NULL);
r3_tree_insert_path(n, "/qux/corge/garply", NULL, NULL);
r3_tree_insert_path(n, "/qux/grault/foo", NULL, NULL);
r3_tree_insert_path(n, "/qux/grault/bar", NULL, NULL);
r3_tree_insert_path(n, "/qux/grault/baz", NULL, NULL);
r3_tree_insert_path(n, "/qux/grault/quux", NULL, NULL);
r3_tree_insert_path(n, "/qux/grault/corge", NULL, NULL);
r3_tree_insert_path(n, "/qux/grault/garply", NULL, NULL);
r3_tree_insert_path(n, "/qux/garply/foo", NULL, NULL);
r3_tree_insert_path(n, "/qux/garply/bar", NULL, NULL);
r3_tree_insert_path(n, "/qux/garply/baz", NULL, NULL);
r3_tree_insert_path(n, "/qux/garply/quux", NULL, NULL);
r3_tree_insert_path(n, "/qux/garply/corge", NULL, NULL);
r3_tree_insert_path(n, "/qux/garply/grault", NULL, NULL);
r3_tree_insert_path(n, "/quux/foo/bar", NULL, NULL);
r3_tree_insert_path(n, "/quux/foo/baz", NULL, NULL);
r3_tree_insert_path(n, "/quux/foo/qux", NULL, NULL);
r3_tree_insert_path(n, "/quux/foo/corge", NULL, NULL);
r3_tree_insert_path(n, "/quux/foo/grault", NULL, NULL);
r3_tree_insert_path(n, "/quux/foo/garply", NULL, NULL);
r3_tree_insert_path(n, "/quux/bar/foo", NULL, NULL);
r3_tree_insert_path(n, "/quux/bar/baz", NULL, NULL);
r3_tree_insert_path(n, "/quux/bar/qux", NULL, NULL);
r3_tree_insert_path(n, "/quux/bar/corge", NULL, NULL);
r3_tree_insert_path(n, "/quux/bar/grault", NULL, NULL);
r3_tree_insert_path(n, "/quux/bar/garply", NULL, NULL);
r3_tree_insert_path(n, "/quux/baz/foo", NULL, NULL);
r3_tree_insert_path(n, "/quux/baz/bar", NULL, NULL);
r3_tree_insert_path(n, "/quux/baz/qux", NULL, NULL);
r3_tree_insert_path(n, "/quux/baz/corge", NULL, NULL);
r3_tree_insert_path(n, "/quux/baz/grault", NULL, NULL);
r3_tree_insert_path(n, "/quux/baz/garply", NULL, NULL);
r3_tree_insert_path(n, "/quux/qux/foo", NULL, NULL);
r3_tree_insert_path(n, "/quux/qux/bar", NULL, NULL);
r3_tree_insert_path(n, "/quux/qux/baz", NULL, NULL);
r3_tree_insert_path(n, "/quux/qux/corge", NULL, NULL);
r3_tree_insert_path(n, "/quux/qux/grault", NULL, NULL);
r3_tree_insert_path(n, "/quux/qux/garply", NULL, NULL);
r3_tree_insert_path(n, "/quux/corge/foo", NULL, NULL);
r3_tree_insert_path(n, "/quux/corge/bar", NULL, NULL);
r3_tree_insert_path(n, "/quux/corge/baz", NULL, NULL);
r3_tree_insert_path(n, "/quux/corge/qux", NULL, NULL);
r3_tree_insert_path(n, "/quux/corge/grault", NULL, NULL);
r3_tree_insert_path(n, "/quux/corge/garply", NULL, NULL);
r3_tree_insert_path(n, "/quux/grault/foo", NULL, NULL);
r3_tree_insert_path(n, "/quux/grault/bar", NULL, NULL);
r3_tree_insert_path(n, "/quux/grault/baz", NULL, NULL);
r3_tree_insert_path(n, "/quux/grault/qux", NULL, NULL);
r3_tree_insert_path(n, "/quux/grault/corge", NULL, NULL);
r3_tree_insert_path(n, "/quux/grault/garply", NULL, NULL);
r3_tree_insert_path(n, "/quux/garply/foo", NULL, NULL);
r3_tree_insert_path(n, "/quux/garply/bar", NULL, NULL);
r3_tree_insert_path(n, "/quux/garply/baz", NULL, NULL);
r3_tree_insert_path(n, "/quux/garply/qux", NULL, NULL);
r3_tree_insert_path(n, "/quux/garply/corge", NULL, NULL);
r3_tree_insert_path(n, "/quux/garply/grault", NULL, NULL);
r3_tree_insert_path(n, "/corge/foo/bar", NULL, NULL);
r3_tree_insert_path(n, "/corge/foo/baz", NULL, NULL);
r3_tree_insert_path(n, "/corge/foo/qux", NULL, NULL);
r3_tree_insert_path(n, "/corge/foo/quux", NULL, NULL);
r3_tree_insert_path(n, "/corge/foo/grault", NULL, NULL);
r3_tree_insert_path(n, "/corge/foo/garply", NULL, NULL);
r3_tree_insert_path(n, "/corge/bar/foo", NULL, NULL);
r3_tree_insert_path(n, "/corge/bar/baz", NULL, NULL);
r3_tree_insert_path(n, "/corge/bar/qux", NULL, NULL);
r3_tree_insert_path(n, "/corge/bar/quux", NULL, NULL);
r3_tree_insert_path(n, "/corge/bar/grault", NULL, NULL);
r3_tree_insert_path(n, "/corge/bar/garply", NULL, NULL);
r3_tree_insert_path(n, "/corge/baz/foo", NULL, NULL);
r3_tree_insert_path(n, "/corge/baz/bar", NULL, NULL);
r3_tree_insert_path(n, "/corge/baz/qux", NULL, NULL);
r3_tree_insert_path(n, "/corge/baz/quux", NULL, NULL);
r3_tree_insert_path(n, "/corge/baz/grault", NULL, NULL);
r3_tree_insert_path(n, "/corge/baz/garply", NULL, NULL);
r3_tree_insert_path(n, "/corge/qux/foo", NULL, NULL);
r3_tree_insert_path(n, "/corge/qux/bar", NULL, NULL);
r3_tree_insert_path(n, "/corge/qux/baz", NULL, NULL);
r3_tree_insert_path(n, "/corge/qux/quux", NULL, NULL);
r3_tree_insert_path(n, "/corge/qux/grault", NULL, NULL);
r3_tree_insert_path(n, "/corge/qux/garply", NULL, NULL);
r3_tree_insert_path(n, "/corge/quux/foo", NULL, NULL);
r3_tree_insert_path(n, "/corge/quux/bar", NULL, NULL);
r3_tree_insert_path(n, "/corge/quux/baz", NULL, NULL);
r3_tree_insert_path(n, "/corge/quux/qux", NULL, NULL);
r3_tree_insert_path(n, "/corge/quux/grault", NULL, NULL);
r3_tree_insert_path(n, "/corge/quux/garply", NULL, NULL);
r3_tree_insert_path(n, "/corge/grault/foo", NULL, NULL);
r3_tree_insert_path(n, "/corge/grault/bar", NULL, NULL);
r3_tree_insert_path(n, "/corge/grault/baz", NULL, NULL);
r3_tree_insert_path(n, "/corge/grault/qux", NULL, NULL);
r3_tree_insert_path(n, "/corge/grault/quux", NULL, NULL);
r3_tree_insert_path(n, "/corge/grault/garply", NULL, NULL);
r3_tree_insert_path(n, "/corge/garply/foo", NULL, NULL);
r3_tree_insert_path(n, "/corge/garply/bar", NULL, NULL);
r3_tree_insert_path(n, "/corge/garply/baz", NULL, NULL);
r3_tree_insert_path(n, "/corge/garply/qux", NULL, NULL);
r3_tree_insert_path(n, "/corge/garply/quux", NULL, NULL);
r3_tree_insert_path(n, "/corge/garply/grault", NULL, NULL);
r3_tree_insert_path(n, "/grault/foo/bar", NULL, NULL);
r3_tree_insert_path(n, "/grault/foo/baz", NULL, NULL);
r3_tree_insert_path(n, "/grault/foo/qux", NULL, NULL);
r3_tree_insert_path(n, "/grault/foo/quux", NULL, NULL);
r3_tree_insert_path(n, "/grault/foo/corge", NULL, NULL);
r3_tree_insert_path(n, "/grault/foo/garply", NULL, NULL);
r3_tree_insert_path(n, "/grault/bar/foo", NULL, NULL);
r3_tree_insert_path(n, "/grault/bar/baz", NULL, NULL);
r3_tree_insert_path(n, "/grault/bar/qux", NULL, NULL);
r3_tree_insert_path(n, "/grault/bar/quux", NULL, NULL);
r3_tree_insert_path(n, "/grault/bar/corge", NULL, NULL);
r3_tree_insert_path(n, "/grault/bar/garply", NULL, NULL);
r3_tree_insert_path(n, "/grault/baz/foo", NULL, NULL);
r3_tree_insert_path(n, "/grault/baz/bar", NULL, NULL);
r3_tree_insert_path(n, "/grault/baz/qux", NULL, NULL);
r3_tree_insert_path(n, "/grault/baz/quux", NULL, NULL);
r3_tree_insert_path(n, "/grault/baz/corge", NULL, NULL);
r3_tree_insert_path(n, "/grault/baz/garply", NULL, NULL);
r3_tree_insert_path(n, "/grault/qux/foo", NULL, NULL);
r3_tree_insert_path(n, "/grault/qux/bar", NULL, NULL);
r3_tree_insert_path(n, "/grault/qux/baz", NULL, NULL);
r3_tree_insert_path(n, "/grault/qux/quux", NULL, NULL);
r3_tree_insert_path(n, "/grault/qux/corge", NULL, NULL);
r3_tree_insert_path(n, "/grault/qux/garply", NULL, NULL);
r3_tree_insert_path(n, "/grault/quux/foo", NULL, NULL);
r3_tree_insert_path(n, "/grault/quux/bar", NULL, NULL);
r3_tree_insert_path(n, "/grault/quux/baz", NULL, NULL);
r3_tree_insert_path(n, "/grault/quux/qux", NULL, NULL);
r3_tree_insert_path(n, "/grault/quux/corge", NULL, NULL);
r3_tree_insert_path(n, "/grault/quux/garply", NULL, NULL);
r3_tree_insert_path(n, "/grault/corge/foo", NULL, NULL);
r3_tree_insert_path(n, "/grault/corge/bar", NULL, NULL);
r3_tree_insert_path(n, "/grault/corge/baz", NULL, NULL);
r3_tree_insert_path(n, "/grault/corge/qux", NULL, NULL);
r3_tree_insert_path(n, "/grault/corge/quux", NULL, NULL);
r3_tree_insert_path(n, "/grault/corge/garply", NULL, NULL);
r3_tree_insert_path(n, "/grault/garply/foo", NULL, NULL);
r3_tree_insert_path(n, "/grault/garply/bar", NULL, NULL);
r3_tree_insert_path(n, "/grault/garply/baz", NULL, NULL);
r3_tree_insert_path(n, "/grault/garply/qux", NULL, NULL);
r3_tree_insert_path(n, "/grault/garply/quux", NULL, NULL);
r3_tree_insert_path(n, "/grault/garply/corge", NULL, NULL);
r3_tree_insert_path(n, "/garply/foo/bar", NULL, NULL);
r3_tree_insert_path(n, "/garply/foo/baz", NULL, NULL);
r3_tree_insert_path(n, "/garply/foo/qux", NULL, NULL);
r3_tree_insert_path(n, "/garply/foo/quux", NULL, NULL);
r3_tree_insert_path(n, "/garply/foo/corge", NULL, NULL);
r3_tree_insert_path(n, "/garply/foo/grault", NULL, NULL);
r3_tree_insert_path(n, "/garply/bar/foo", NULL, NULL);
r3_tree_insert_path(n, "/garply/bar/baz", NULL, NULL);
r3_tree_insert_path(n, "/garply/bar/qux", NULL, NULL);
r3_tree_insert_path(n, "/garply/bar/quux", NULL, NULL);
r3_tree_insert_path(n, "/garply/bar/corge", NULL, NULL);
r3_tree_insert_path(n, "/garply/bar/grault", NULL, NULL);
r3_tree_insert_path(n, "/garply/baz/foo", NULL, NULL);
r3_tree_insert_path(n, "/garply/baz/bar", NULL, NULL);
r3_tree_insert_path(n, "/garply/baz/qux", NULL, NULL);
r3_tree_insert_path(n, "/garply/baz/quux", NULL, NULL);
r3_tree_insert_path(n, "/garply/baz/corge", NULL, NULL);
r3_tree_insert_path(n, "/garply/baz/grault", NULL, NULL);
r3_tree_insert_path(n, "/garply/qux/foo", NULL, NULL);
r3_tree_insert_path(n, "/garply/qux/bar", NULL, NULL);
r3_tree_insert_path(n, "/garply/qux/baz", NULL, NULL);
r3_tree_insert_path(n, "/garply/qux/quux", NULL, NULL);
r3_tree_insert_path(n, "/garply/qux/corge", NULL, NULL);
r3_tree_insert_path(n, "/garply/qux/grault", NULL, NULL);
r3_tree_insert_path(n, "/garply/quux/foo", NULL, NULL);
r3_tree_insert_path(n, "/garply/quux/bar", NULL, NULL);
r3_tree_insert_path(n, "/garply/quux/baz", NULL, NULL);
r3_tree_insert_path(n, "/garply/quux/qux", NULL, NULL);
r3_tree_insert_path(n, "/garply/quux/corge", NULL, NULL);
r3_tree_insert_path(n, "/garply/quux/grault", NULL, NULL);
r3_tree_insert_path(n, "/garply/corge/foo", NULL, NULL);
r3_tree_insert_path(n, "/garply/corge/bar", NULL, NULL);
r3_tree_insert_path(n, "/garply/corge/baz", NULL, NULL);
r3_tree_insert_path(n, "/garply/corge/qux", NULL, NULL);
r3_tree_insert_path(n, "/garply/corge/quux", NULL, NULL);
r3_tree_insert_path(n, "/garply/corge/grault", NULL, NULL);
r3_tree_insert_path(n, "/garply/grault/foo", NULL, NULL);
r3_tree_insert_path(n, "/garply/grault/bar", NULL, NULL);
r3_tree_insert_path(n, "/garply/grault/baz", NULL, NULL);
r3_tree_insert_path(n, "/garply/grault/qux", NULL, NULL);
r3_tree_insert_path(n, "/garply/grault/quux", NULL, NULL);
r3_tree_insert_path(n, "/garply/grault/corge", NULL, NULL);


    r3_tree_compile(n);
    // r3_tree_dump(n, 0);
    // match_entry *entry = calloc( sizeof(entry) , 1 );

    node *m;
    m = r3_tree_match(n , "/qux/bar/corge", strlen("/qux/bar/corge"), NULL);
    fail_if( m == NULL );
    // r3_tree_dump( m, 0 );
    ck_assert_int_eq( *((int*) m->data), 999 );


    printf("Benchmarking...\n");
    BENCHMARK(string_dispatch)
    r3_tree_match(n , "/qux/bar/corge", strlen("/qux/bar/corge"), NULL);
    END_BENCHMARK()

    bench_print_summary(&B);

    FILE *fp = fopen("bench_str.csv", "a+");
    fprintf(fp, "%ld,%.2f\n", unixtime(), (B.N * B.R) / (B.end - B.start));
    fclose(fp);

}
END_TEST


Suite* r3_suite (void) {
        Suite *suite = suite_create("blah");

        TCase *tcase = tcase_create("testcase");
        tcase_add_test(tcase, test_str_array);
        tcase_add_test(tcase, test_ltrim_slash);
        tcase_add_test(tcase, test_r3_node_construct_uniq);
        tcase_add_test(tcase, test_r3_node_find_edge);
        tcase_add_test(tcase, test_r3_tree_insert_pathl);
        tcase_add_test(tcase, test_compile_slug);
        tcase_add_test(tcase, test_compile);
        tcase_add_test(tcase, test_route_cmp);
        tcase_add_test(tcase, test_insert_route);
        tcase_add_test(tcase, test_pcre_pattern_simple);
        tcase_add_test(tcase, test_pcre_pattern_more);


        tcase_add_test(tcase, benchmark_str);

        suite_add_tcase(suite, tcase);

        return suite;
}

int main (int argc, char *argv[]) {
        int number_failed;
        Suite *suite = r3_suite();
        SRunner *runner = srunner_create(suite);
        srunner_run_all(runner, CK_NORMAL);
        number_failed = srunner_ntests_failed(runner);
        srunner_free(runner);
        return number_failed;
}
