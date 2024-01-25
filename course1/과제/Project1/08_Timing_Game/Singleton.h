#pragma once
#define SINGLE(type)	public:							\
							static type *GetInstance();	\
						private:						\
							type() = default;			\
							~type();					\
						