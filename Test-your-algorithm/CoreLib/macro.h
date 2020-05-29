#pragma once

/*
	Makes the class singleton
*/
#define THIS_CLASS_IS_SINGLETON(className)				\
	public:												\
		static className* GetInstance()					\
		{												\
			static className instance;					\
			return &instance;							\
		}												\
														\
	private:											\
		className() = default;							\
		~className() = default;							\
		className(const className& rhs) = delete;		\
		className(const className&& rhs) = delete;		\
		void operator=(const className& rhs) = delete;	\
		void operator=(const className&& rhs) = delete;

/*
	Macro for character array
*/
#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)
#define CONCAT_STRING(a,b,c) a##b##c
