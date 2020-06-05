

ALTER TABLE CP_ALGO_PROPERTY MODIFY PROP_NAME VARCHAR2(48 BYTE); 
ALTER TABLE CP_ALGO_PROPERTY_ARCHIVE MODIFY PROP_NAME VARCHAR2(48 BYTE); 
ALTER TABLE CP_ALGO_PROPERTY_ARCHIVE  MODIFY (PROP_VALUE NULL);

ALTER TABLE CP_COMP_PROPERTY MODIFY PROP_NAME VARCHAR2(48 BYTE);
ALTER TABLE CP_COMP_PROPERTY_ARCHIVE MODIFY PROP_NAME VARCHAR2(48 BYTE);


GRANT SELECT on ALARM_EVENTIDSEQ to DECODES_ROLE,CALC_DEFINITION_ROLE,SAVOIR_FAIRE;
GRANT SELECT on ALARM_GROUPIDSEQ to DECODES_ROLE,CALC_DEFINITION_ROLE,SAVOIR_FAIRE;
GRANT SELECT on ALARM_LIMIT_SETIDSEQ to DECODES_ROLE,CALC_DEFINITION_ROLE,SAVOIR_FAIRE;
GRANT SELECT on ALARM_SCREENINGIDSEQ to DECODES_ROLE,CALC_DEFINITION_ROLE,SAVOIR_FAIRE;

GRANT INSERT,UPDATE,DELETE on HDB_LOADING_APPLICATION  to CALC_DEFINITION_ROLE;

-- verify below just incase 
--CREATE PUBLIC SYNONYM ALARM_LIMIT_SET FOR &CP_OWNER..ALARM_LIMIT_SET;
--CREATE SEQUENCE ALARM_GROUPIdSeq nocache;
--CREATE PUBLIC SYNONYM ALARM_GROUPIdSeq FOR &CP_OWNER..ALARM_GROUPIdSeq;


--update version table
INSERT INTO TSDB_PROPERTY (PROP_NAME, PROP_VALUE) VALUES ('4.1.3', 'CP_ALGO_PROPERTY prop_name increased and its archive fixed');
/

--Commit
commit;

