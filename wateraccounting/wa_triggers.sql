set verify off
spool wa_triggers.out

@ref_wa_annotation_triggers.trg  &1
@ref_wa_model_site_config_triggers.trg &1
@ref_wa_exchange_triggers.trg &1
@ref_wa_exchange_payback_triggers.trg &1
@ref_wa_supply_triggers.trg &1

spool off
