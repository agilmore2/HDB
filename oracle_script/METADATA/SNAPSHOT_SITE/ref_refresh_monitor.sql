DROP TABLE ref_refresh_monitor;

CREATE TABLE ref_refresh_monitor
(message VARCHAR2 (200),
 success_code NUMBER(1)
);

grant select on ref_refresh_monitor to public;

drop public synonym ref_refresh_monitor;
create public synonym ref_refresh_monitor for &1..ref_refresh_monitor;
quit;
