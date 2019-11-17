set linesize 70
set pagesize 50
column display_date heading 'Date'
column release heading 'Released (acft)' format 9999999
set feedback off
break on report
compute sum of release on report

TTITLE 'Lake Powell'
BTITLE 'All data is provisional and subject to review and modification'

spool powell_releases.out

select to_char(start_date_time,'dd-Mon-yyyy') display_date,
round(value,0) release
 from r_day where site_datatype_id = 1920 and start_date_time >= trunc(sysdate-1,'MON')
-- from r_day where site_datatype_id = 1872 and start_date_time >= trunc(sysdate-1,'MON')
order by 1;

spool off
exit
