------- vm_init -------
------- vm_anon_init -------
------- vm_anon_init end -------
------- vm_file_init -------
------- vm_file_init end -------
------- register_inspect_intr -------
------- register_inspect_intr end -------
------- vm_init end -------
Boot complete.
Putting 'mmap-bad-fd' into the file system...
Executing 'mmap-bad-fd':

------- process_create_initd -------
------- supplemental_page_table_init -------
------- hash_init -------
------- hash_clear -------
------- hash_clear end -------
------- hash_init end (h->buckets != NULL) -------
------- supplemental_page_table_init end -------
------- process_exec -------
------- process_cleanup -------
------- supplemental_page_table_kill -------
------- hash_clear -------
------- hash_clear end -------
------- supplemental_page_table_kill end -------
------- process_cleanup end -------
------- load -------
------- load_segment -------
------- vm_alloc_page_with_initializer -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem null -------
------- uninit_new -------
------- uninit_new end -------
------- vm_alloc_page_with_initializer end -------
------- spt_insert_page -------
------- hash_insert -------
------- hash_insert end -------
------- spt_insert_page end -------
------- load_segment end true -------
------- load_segment -------
------- vm_alloc_page_with_initializer -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem null -------
------- uninit_new -------
------- uninit_new end -------
------- vm_alloc_page_with_initializer end -------
------- spt_insert_page -------
------- hash_insert -------
------- hash_insert end -------
------- spt_insert_page end -------
------- vm_alloc_page_with_initializer -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem null -------
------- uninit_new -------
------- uninit_new end -------
------- vm_alloc_page_with_initializer end -------
------- spt_insert_page -------
------- hash_insert -------
------- hash_insert end -------
------- spt_insert_page end -------
------- vm_alloc_page_with_initializer -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem null -------
------- uninit_new -------
------- uninit_new end -------
------- vm_alloc_page_with_initializer end -------
------- spt_insert_page -------
------- hash_insert -------
------- hash_insert end -------
------- spt_insert_page end -------
------- vm_alloc_page_with_initializer -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem null -------
------- uninit_new -------
------- uninit_new end -------
------- vm_alloc_page_with_initializer end -------
------- spt_insert_page -------
------- hash_insert -------
------- hash_insert end -------
------- spt_insert_page end -------
------- vm_alloc_page_with_initializer -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem null -------
------- uninit_new -------
------- uninit_new end -------
------- vm_alloc_page_with_initializer end -------
------- spt_insert_page -------
------- hash_insert -------
------- hash_insert end -------
------- spt_insert_page end -------
------- load_segment end true -------
------- load_segment -------
------- vm_alloc_page_with_initializer -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem null -------
------- uninit_new -------
------- uninit_new end -------
------- vm_alloc_page_with_initializer end -------
------- spt_insert_page -------
------- hash_insert -------
------- hash_insert end -------
------- spt_insert_page end -------
------- vm_alloc_page_with_initializer -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem null -------
------- uninit_new -------
------- uninit_new end -------
------- vm_alloc_page_with_initializer end -------
------- spt_insert_page -------
------- hash_insert -------
------- hash_insert end -------
------- spt_insert_page end -------
------- load_segment end true -------
------- vm_alloc_page_with_initializer -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem null -------
------- uninit_new -------
------- uninit_new end -------
------- vm_alloc_page_with_initializer end -------
------- spt_insert_page -------
------- hash_insert -------
------- hash_insert end -------
------- spt_insert_page end -------
------- vm_claim_page -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem not null -------
------- vm_claim_page end -------
------- vm_do_claim_page -------
------- vm_get_frame -------
------- vm_get_frame end -------
------- vm_do_claim_page end page->frame->kva -------
------- uninit_initialize -------
------- uninit_initialize end -------
------- anon_initializer -------
------- anon_initializer end -------
------- load end -------
------- vm_try_handle_fault -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem not null -------
------- vm_try_handle_fault end (not_present) -------
------- vm_do_claim_page -------
------- vm_get_frame -------
------- vm_get_frame end -------
------- vm_do_claim_page end page->frame->kva -------
------- uninit_initialize -------
------- uninit_initialize end -------
------- anon_initializer -------
------- anon_initializer end -------
------- lazy_load_segment -------
------- lazy_load_segment end true -------
------- vm_try_handle_fault -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem not null -------
------- vm_try_handle_fault end (not_present) -------
------- vm_do_claim_page -------
------- vm_get_frame -------
------- vm_get_frame end -------
------- vm_do_claim_page end page->frame->kva -------
------- uninit_initialize -------
------- uninit_initialize end -------
------- anon_initializer -------
------- anon_initializer end -------
------- lazy_load_segment -------
------- lazy_load_segment end true -------
------- vm_try_handle_fault -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem not null -------
------- vm_try_handle_fault end (not_present) -------
------- vm_do_claim_page -------
------- vm_get_frame -------
------- vm_get_frame end -------
------- vm_do_claim_page end page->frame->kva -------
------- uninit_initialize -------
------- uninit_initialize end -------
------- anon_initializer -------
------- anon_initializer end -------
------- lazy_load_segment -------
------- lazy_load_segment end true -------
------- vm_try_handle_fault -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem not null -------
------- vm_try_handle_fault end (not_present) -------
------- vm_do_claim_page -------
------- vm_get_frame -------
------- vm_get_frame end -------
------- vm_do_claim_page end page->frame->kva -------
------- uninit_initialize -------
------- uninit_initialize end -------
------- anon_initializer -------
------- anon_initializer end -------
------- lazy_load_segment -------
------- lazy_load_segment end true -------
------- vm_try_handle_fault -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem not null -------
------- vm_try_handle_fault end (not_present) -------
------- vm_do_claim_page -------
------- vm_get_frame -------
------- vm_get_frame end -------
------- vm_do_claim_page end page->frame->kva -------
------- uninit_initialize -------
------- uninit_initialize end -------
------- anon_initializer -------
------- anon_initializer end -------
------- lazy_load_segment -------
------- lazy_load_segment end true -------
------- vm_try_handle_fault -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem not null -------
------- vm_try_handle_fault end (not_present) -------
------- vm_do_claim_page -------
------- vm_get_frame -------
------- vm_get_frame end -------
------- vm_do_claim_page end page->frame->kva -------
------- uninit_initialize -------
------- uninit_initialize end -------
------- anon_initializer -------
------- anon_initializer end -------
------- lazy_load_segment -------
------- lazy_load_segment end true -------
------- syscall handler -------
SYS_WRITE
(mmap-bad-fd) begin
-------------------------------


------- vm_try_handle_fault -------
------- spt_find_page -------
------- hash_find -------
------- spt_find_page end hash_elem not null -------
------- vm_try_handle_fault end (not_present) -------
------- vm_do_claim_page -------
------- vm_get_frame -------
------- vm_get_frame end -------
------- vm_do_claim_page end page->frame->kva -------
------- uninit_initialize -------
------- uninit_initialize end -------
------- anon_initializer -------
------- anon_initializer end -------
------- lazy_load_segment -------
------- lazy_load_segment end true -------
------- syscall handler -------
SYS_WRITE
(mmap-bad-fd) try to mmap invalid fd
-------------------------------


------- syscall handler -------
default;
-------------------------------


------- syscall handler -------
SYS_WRITE
(mmap-bad-fd) try to mmap invalid fd: FAILED
-------------------------------


------- syscall handler -------
SYS_EXIT

------- process_cleanup -------
------- supplemental_page_table_kill -------
------- hash_clear -------
------- uninit_destroy -------
------- uninit_destroy end -------
------- anon_destroy -------
------- anon_destroy end -------
------- anon_destroy -------
------- anon_destroy end -------
------- anon_destroy -------
------- anon_destroy end -------
------- anon_destroy -------
------- anon_destroy end -------
------- anon_destroy -------
------- anon_destroy end -------
------- anon_destroy -------
------- anon_destroy end -------
------- anon_destroy -------
------- anon_destroy end -------
------- anon_destroy -------
------- anon_destroy end -------
------- hash_clear end -------
------- supplemental_page_table_kill end -------
------- process_cleanup end -------