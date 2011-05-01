
create table users(
  id          integer primary key,
  created_at  datetime,
  updated_at  datetime,
  deleted_at  datetime,
  username    char(63)
);

create index users_created_at on users(created_at);
create index users_updated_at on users(updated_at);
create index users_deleted_at on users(deleted_at);
