alter table    ref_hm_filetype
add
   constraint  ref_hm_filetype_ck1
   check       (hm_filetype = 'A' or hm_filetype = 'D');
