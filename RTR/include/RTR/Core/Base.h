#define RTR_NON_COPYABLE(type) \
	type(const type&) = delete; \
	type& operator=(const type&) = delete;