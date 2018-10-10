
#include <type_traits>

namespace std
{

  // integral constants

  template<bool _BoolVal>
    using mp_bool = std::integral_constant<bool, _BoolVal>;
  template<int _IntVal>
    using mp_int = std::integral_constant<int, _IntVal>;
  template<std::size_t _Num>
    using mp_size_t = std::integral_constant<std::size_t, _Num>;

  using mp_true = mp_bool<true>;
  using mp_false = mp_bool<false>;

  template<typename _Tp>
    using mp_to_bool = mp_bool<static_cast<bool>(_Tp::value)>;
  template<typename _Tp>
    using mp_not = mp_bool<!_Tp::value>;

  // utility components

  template<typename _Tp>
    struct mp_identity;
  template<typename _Tp>
    using mp_identity_t = typename mp_identity<_Tp>::type;

  template<typename... _Tp>
    struct mp_inherit;

  namespace __detail
  {
    template<bool _Cond, typename _Tp, typename _Fp>
      struct __if_c_impl;

    template<typename _Tp, typename _Fp>
      struct __if_c_impl<true, _Tp, _Fp>
      { using type = _Tp; };

    template<typename _Tp, typename _Fp>
      struct __if_c_impl<false, _Tp, _Fp>
      { using type = _Fp; };
  }

  template<bool _Cond, typename _Tp, typename _Fp>
    using mp_if_c = typename __detail::__if_c_impl<_Cond, _Tp, _Fp>::type;
  template<typename _Cond, typename _Tp, typename _Fp>
    using mp_if = mp_if_c<static_cast<bool>(_Cond::value), _Tp, _Fp>;

  namespace __detail
  {
    template<bool _Cond, typename _Tp, template<typename...> typename _Fun,
	     typename... _Args>
      struct __eval_if_c_impl;

    template<typename _Tp, template<typename...> typename _Fun,
	     typename... _Args>
      struct __eval_if_c_impl<true, _Tp, _Fun, _Args...>
      { using type = _Tp; };

    template<typename _Tp, template<typename...> typename _Fun,
	     typename... _Args>
      struct __eval_if_c_impl<false, _Tp, _Fun, _Args...>
      { using type = _Fun<_Args...>; };

    template<bool _Cond, typename _Tp, typename... _Res>
      struct __cond_impl;

    template<typename _Tp, typename... _Res>
      struct __cond_impl<true, _Tp, _Res...>
      { using type = _Tp; };

    template<typename _Tp, typename... _Res>
      struct __cond_impl<false, _Tp, _Res...>
      { using type = _Res...; };
  }

  template<bool _Cond, typename _Tp,
	   template<typename...> typename _Fun, typename... _Args>
    using mp_eval_if_c
	    = typename __detail::__eval_if_c_impl<_Cond, _Tp, _Fun,
						  _Args...>::type;
  template<typename _Cond, typename _Tp,
	   template<typename...> typename _Fun, typename... _Args>
    using mp_eval_if
	    = mp_eval_if_c<static_cast<bool>(_Cond::value), _Tp, _Fun,
			   _Args...>;
  template<typename _Cond, typename _Tp, typename _Quote, typename... _Args>
    using mp_eval_if_q = mp_eval_if<_Cond, _Tp, _Quote::template fn, _Args...>;

  template<typename _Cond, typename _Tp, typename... _Res>
    using mp_cond = typename __detail::__cond_impl<static_cast<bool>(_Cond::value), _Tp, _Res...>::type;

  namespace __detail
  {
    template<typename... _Tp>
      struct __valid_impl
      { using type = mp_false; };

    template<template<typename...> typename _Fun, typename... _Tp>
      struct __valid_impl<_Fun<_Tp...>>
      { using type = mp_true; };

    template<typename... _Tp>
      struct __defer_impl
      { using type = struct {}; };

    template<template<typename...> typename _Fun, typename... _Tp>
      struct __defer_impl<_Fun<_Tp...>>
      { using type = _Fun<_Tp...>; };
  }

  template<template<typename...> typename _Fun, typename... _Tp>
    using mp_valid = typename __detail::__valid_impl<_Fun, _Tp...>::type;
  template<template<typename...> typename _Fun, typename... _Tp>
    using mp_defer = typename __detail::__defer_impl<_Fun, _Tp...>::type;

  template<template<typename...> typename _Fun>
    struct mp_quote
    {
      template<class... _Tp>
	using fn = typename mp_defer<_Fun, _Tp...>::type;
    };
  template<template<typename...> typename _Fun>
    struct mp_quote_trait
    {
      template<class... _Tp>
	using fn = typename _Fun<_Tp...>::type;
    };
  template<typename _Quote, typename... _Trait>
    using mp_invoke = typename _Quote::template fn<_Trait...>;

  // list operations

  template<typename... _Tp>
    struct mp_list
    { };

  template<typename _Tp, _Tp... _IntVal>
    using mp_list_c = mp_list<integral_constant<_Tp, _IntVal>...>;

  namespace __detail
  {
    template<typename _List>
      struct __is_list_impl
      { using type = mp_false; };

    template<typename... _Tp>
      struct __is_list_impl<mp_list<_Tp...>>
      { using type = mp_true; };
  }

  template<typename _List>
    using mp_is_list = typename __detail::__is_list_impl<_List>::type;

  namespace __detail
  {
    template<typename _List>
      struct __size_impl;

    template<typename... _Tp>
      struct __size_impl<mp_list<_Tp...>>
      { using type = std::integral_constant<std::size_t, sizeof...(_Tp)>; };
  }

  template<typename _List>
    using mp_size = typename __detail::__size_impl<_List>::type;
  template<typename _List>
    using mp_empty = mp_bool<mp_size<_List>::value == 0>;

  namespace __detail
  {
    template<typename _List1, typename... _Tp1,
	     typename _List2, typename... _Tp2>
      struct __assign_impl;

    template<template<typename...> typename _List1, typename... _Tp1,
	     template<typename...> typename _List2, typename... _Tp2>
      struct __assign_impl
      { using type = _List1<_Tp2...>; }
  }
help!!!
  template<typename _List1, typename _List2>
    using mp_assign = typename __detail::__assign_impl<>::type;
  template<typename _List>
    using mp_clear = mp_assign<_List, mp_list<>>;

  namespace __detail
  {
    template<typename _List>
      struct __front_impl;

    template<template<typename...> typename _List,
	     typename _Tp1, typename... _Tp>
      struct __front_impl<_List<_Tp1, _Tp...>>
      { using type = _Tp1; };

    template<typename _List>
      struct __pop_front_impl;

    template<template<typename...> typename _List,
	     typename _Tp1, typename... _Tp>
      struct __pop_front_impl<_List<_Tp1, _Tp...>>
      { using type = mp_list<_Tp...>; }; // _List<_Tp...>?

    template<typename _List>
      struct __second_impl;

    template<template<typename...> typename _List,
	     typename _Tp1, typename _Tp2, typename... _Tp>
      struct __second_impl<_List<_Tp1, _Tp2, _Tp...>>
      { using type = _Tp2; };

    template<typename _List>
      struct __third_impl;

    template<template<typename...> typename _List,
	     typename _Tp1, typename _Tp2, typename _Tp3, typename... _Tp>
      struct __third_impl<_List<_Tp1, _Tp2, _Tp3, _Tp...>>
      { using type = _Tp3; };
  }

  template<typename _List>
    using mp_front = typename __detail::__front_impl<_List>::type;
  template<typename _List>
    using mp_pop_front = typename __detail::__pop_front_impl<_List>::type;
  template<typename _List>
    using mp_first = mp_front<_List>;
  template<typename _List>
    using mp_rest = mp_pop_front<_List>;
  template<typename _List>
    using mp_second = typename __detail::__second_impl<_List>::type;
  template<typename _List>
    using mp_third = typename __detail::__third_impl<_List>::type;

  namespace __detail
  {
    template<typename _List, typename... _Tp>
      struct __push_front_impl;

    template<template<typename...> typename _List, typename... _Up,
	     typename... _Tp>
      struct __push_front_impl<_List<_Up...>, _Tp...>
      { using type = _List<_Tp..., _Up...>; };

    template<typename _List, typename... _Tp>
      struct __push_back_impl;

    template<template<typename...> typename _List, typename... _Up,
	     typename... _Tp>
      struct __push_back_impl<_List<_Up...>, _Tp...>
      { using type = _List<_Up..., _Tp...>; };
  }

  template<typename _List, typename... _Tp>
    using mp_push_front
	    = typename __detail::__push_front_impl<_List, _Tp...>::type;
  template<typename _List, typename... _Tp>
    using mp_push_back
	    = typename __detail::__push_back_impl<_List, _Tp...>::type;

  namespace __detail
  {
    template<typename _List, template<typename...> typename Y>
      struct __rename_impl;

    template<template<typename...> typename _List, typename... _Tp,
	     template<typename...> typename Y>
      struct __rename_impl<_List<_Tp...>, Y>
      { using type = Y<_Tp...>; };
  }

  template<typename _List, template<typename...> typename Y>
    using mp_rename = typename __detail::__rename_impl<_List, Y>::type;
  template<template<typename...> typename _Fun, typename _List>
    using mp_apply = mp_rename<_List, _Fun>;
  template<typename _Quote, typename _List>
    using mp_apply_q = mp_apply<_Quote::template fn, _List>;

  namespace __detail
  {
    template<typename... _List>
      struct __append_impl;

    template<>
      struct __append_impl<>
      { using type = mp_list<>; };

    template<template<typename...> typename _List, typename... _Tp>
      struct __append_impl<_List<_Tp...>>
      { using type = _List<_Tp...>; };

    template<template<typename...> typename _List1, typename... _Tp1,
	     template<typename...> typename _List2, typename... _Tp2,
	     typename... _Listbum>
      struct __append_impl<_List1<_Tp1...>, _List2<_Tp2...>, _Listbum...>
      { using type = mp_append<_List1<_Tp1..., _Tp2...>, _Listbum...>; };
  }

  template<typename... _List>
    using mp_append = typename __detail::__append_impl<_List...>::type;

  namespace __detail
  {
    template<typename _List, typename _Tp>
      struct __replace_first_impl;

    template<template<typename...> typename _List, typename _Up,
	     typename _Tp1, typename... _Rest>
      struct __replace_first_impl<_List<_Tp1, _Rest...>, _Up>
      { using type = _List<_Up, _Rest...>; };

    template<typename _List, typename _Tp>
      struct __replace_second_impl;

    template<template<typename...> typename _List, typename _Up,
	     typename _Tp1, typename _Tp2, typename... _Rest>
      struct __replace_second_impl<_List<_Tp1, _Tp2, _Rest...>, _Up>
      { using type = _List<_Tp1, _Up, _Rest...>; };

    template<typename _List, typename _Tp>
      struct __replace_third_impl;

    template<template<typename...> typename _List, typename _Up,
	     typename _Tp1, typename _Tp2, typename _Tp3, typename... _Rest>
      struct __replace_third_impl<_List<_Tp1, _Tp2, _Tp3, _Rest...>, _Up>
      { using type = _List<_Tp1, _Tp2, _Up, _Rest...>; };
  }

  template<typename _List, typename _Tp>
    using mp_replace_front
	    = typename __detail::__replace_first_impl<_List, _Tp>::type;
  template<typename _List, typename _Tp>
    using mp_replace_first = mp_replace_front<_List, _Tp>;
  template<typename _List, typename _Tp>
    using mp_replace_second
	    = typename __detail::__replace_second_impl<_List, _Tp>::type;
  template<typename _List, typename _Tp>
    using mp_replace_third
	    = typename __detail::__replace_third_impl<_List, _Tp>::type;

  // algorithms

  namespace __detail
  {
    template<template<typename...> typename _Fun, typename... _List>
      struct __transform_impl;

    template<template<typename...> typename _Fun, typename _List1, typename... _List>
      struct __transform_impl<_Fun, mp_true, _List1, _List...>
      { using type = mp_clear<_List1>; };

    template<template<typename...> typename _Fun, typename... _List>
      struct __transform_impl<_Fun, mp_false, _List...>
      {
	using _First = _Fun<typename __front_impl<_List>::type...>;
	using _Rest = mp_transform<_Fun, typename __pop_front_impl<_List>::type...>;
	using type = mp_push_front<_Rest, _First>;
      };
  }

  template<template<typename...> typename _Fun, typename... _List>
    using mp_transform
	    = typename __detail::__transform_impl<_Fun, mp_empty<_List...>,
						  _List...>::type;
  template<typename _Quote, typename... _List>
    using mp_transform_q = mp_transform<_Quote::template fn, _List...>;
  template<template<typename...> typename _Pred,
	   template<typename...> typename _Fun, typename... _List>
    using mp_transform_if = /*see below*/;
  template<typename Qp, typename Qf, typename... _List>
    using mp_transform_if_q
	    = mp_transform_if<Qp::template fn, Qf::template fn, _List...>;

  template<typename _List, typename _Vp>
    using mp_fill = /*see below*/;

  namespace __detail
  {
    template<typename _List, typename _Vp>
      struct __count_impl;

    template<template<typename...> typename _List, typename... _Tp, typename _Vp>
      struct __count_impl<_List<_Tp...>, _Vp>
      { using type = mp_plus<std::is_same<_Tp, _Vp>...>; };
  }

  template<typename _List, typename _Vp>
    using mp_count = typename __detail::__count_impl<_List, _Vp>;
  template<typename _List, template<typename...> typename _Pred>
    using mp_count_if = /*see below*/;
  template<typename _List, typename _Quote>
    using mp_count_if_q = mp_count_if<_List, _Quote::template fn>;

  template<typename _List, typename _Vp>
    using mp_contains = mp_to_bool<mp_count<_List, _Vp>>;

  namespace __detail
  {
    template<std::size_t _Num, typename... _Tp>
      struct __repeat_c_impl
      {
	using _List1 = typename __repeat_c_impl<_Num / 2, _Tp...>::type;
	using _List2 = typename __repeat_c_impl<_Num % 2, _Tp...>::type;
	using type = mp_append<_List1, _List1, _List2>;
      };

    template<typename... _Tp>
      struct __repeat_c_impl<0, _Tp...>
      { using type = mp_list<>; };

    template<typename... _Tp>
      struct __repeat_c_impl<1, _Tp...>
      { using type = mp_list<_Tp...>; };
  }

  template<typename _List, std::size_t _Num>
    using mp_repeat_c = typename __detail::__repeat_c_impl<_Num, _List>::type;
  template<typename _List, typename _Num>
    using mp_repeat = mp_repeat_c<_List, std::size_t{_Num::value}>;

  template<template<typename...> typename _Fun, typename... _List>
    using mp_product = /*see below*/;
  template<typename _Quote, typename... _List>
    using mp_product_q = mp_product<_Quote::template fn, _List...>;

  namespace __detail
  {
    template<typename _List, typename _List2>
      struct __drop_c_impl;

    template<template<typename...> typename _List, typename... _Tp,
	     template<typename...> typename _List2, typename... _Up>
      struct __drop_c_impl<_List<_Tp...>, _List2<_Up...>>
      {
	template<typename... _Wp>
	  static mp_identity<_List<_Wp...>>
	  __fun(_Up*..., mp_identity<_Wp>*...);

	using _Res = decltype(__fun((mp_identity<_Tp>*)nullptr ...));

	using type = typename _Res::type;
      };
  }

  template<typename _List, std::size_t _Num>
    using mp_drop_c
	    = typename __drop_c_impl<_List, mp_repeat_c<_Num, void>>::type;
  template<typename _List, typename _Num>
    using mp_drop = mp_drop_c<_List, std::size_t{_Num::value}>;

  namespace __detail
  {
    template<typename _Seq>
      struct __from_sequence_impl;

    template<template<typename _Tp, _Tp... _Idx> typename _Seq,
	     typename _Up, _Up... _Jdx>
      struct __from_sequence_impl<_Seq<_Up, _Jdx...>>
      { using type = mp_list<std::integral_constant<_Up, _Jdx>...>; };
  }

  template<typename _Seq>
    using mp_from_sequence = typename __from_sequence_impl<_Seq>::type;
  template<std::size_t _Num>
    using mp_iota_c = mp_from_sequence<std::make_index_sequence<_Num>>;
  template<typename _Num>
    using mp_iota = mp_from_sequence<std::make_integer_sequence<
			std::remove_const_t<decltype(_Num::value)>,
					    _Num::value>>;

  namespace __detail
  {
    template<typename _List, typename _List2>
      struct __at_c_impl;

    template<template<typename...> typename _List, typename... _Tp,
      template<typename...> typename _List2, typename... _Up>
      struct __at_c_impl<_List<_Tp...>, _List2<_Up...>>
      {
	template<typename _Wp>
	  static _Wp
	  __fun(_Up*..., _Wp*, ...);

	using _Res = decltype(__fun((mp_identity<_Tp>*)nullptr ...));

	using type = typename _Res::type;
      };
  }

  template<typename _List, std::size_t _Index>
    using mp_at_c
	    = typename mp_at_c_impl<_List, mp_repeat_c<_Index, void>>::type;
  template<typename _List, typename _Index>
    using mp_at = mp_at_c<_List, std::size_t{_Index::value}>;

  template<typename _List, std::size_t _Num>
    using mp_take_c = /*see below*/;
  template<typename _List, typename _Num>
    using mp_take = mp_take_c<_List, std::size_t{_Num::value}>;

  template<typename _List, std::size_t _Index, typename... _Tp>
    using mp_insert_c
	    = mp_append<mp_take_c<_List, _Index>,
			mp_push_front<mp_drop_c<_List, _Index>, _Tp...>>;
  template<typename _List, typename _Index, typename... _Tp>
    using mp_insert
	    = mp_append<mp_take<_List, _Index>,
			mp_push_front<mp_drop<_List, _Index>, _Tp...>>;

  template<typename _List, std::size_t _Index, std::size_t _Jndex>
    using mp_erase_c = mp_append<mp_take_c<_List, _Index>, mp_drop_c<_List, _Jndex>>;
  template<typename _List, typename _Index, typename _Jndex>
    using mp_erase = mp_append<mp_take<_List, _Index>, mp_drop<_List, _Jndex>>;

  template<typename _List, typename _Vp, typename _Wp>
    using mp_replace = /*see below*/;
  template<typename _List, template<typename...> typename _Pred, typename _Wp>
    using mp_replace_if = /*see below*/;
  template<typename _List, typename _Quote, typename _Wp>
    using mp_replace_if_q = mp_replace_if<_List, _Quote::template fn, _Wp>;
  template<typename _List, std::size_t _Index, typename _Wp>
    using mp_replace_at_c = /*see below*/;
  template<typename _List, typename _Index, typename _Wp>
    using mp_replace_at = mp_replace_at_c<_List, std::size_t{_Index::value}, _Wp>;

  template<typename _List, template<typename...> typename _Pred>
    using mp_copy_if = /*see below*/;
  template<typename _List, typename _Quote>
    using mp_copy_if_q = mp_copy_if<_List, _Quote::template fn>;

  template<typename _List, typename _Vp>
    using mp_remove = /*see below*/;
  template<typename _List, template<typename...> typename _Pred>
    using mp_remove_if = /*see below*/;
  template<typename _List, typename _Quote>
    using mp_remove_if_q = mp_remove_if<_List, _Quote::template fn>;

  template<typename _List, template<typename...> typename _Pred>
    using mp_partition = /*see below*/;
  template<typename _List, typename _Quote>
    using mp_partition_q = mp_partition<_List, _Quote::template fn>;
  template<typename _List, template<typename...> typename _Pred>
    using mp_sort = /*see below*/;
  template<typename _List, typename _Quote>
    using mp_sort_q = mp_sort<_List, _Quote::template fn>;
  template<typename _List, std::size_t _Index, template<typename...> typename _Pred>
    using mp_nth_element_c = /*see below*/;
  template<typename _List, typename _Index, template<typename...> typename _Pred>
    using mp_nth_element = mp_nth_element_c<_List, std::size_t{_Index::value}, _Pred>;
  template<typename _List, typename _Index, typename _Quote>
    using mp_nth_element_q = mp_nth_element<_List, _Index, _Quote::template fn>;
  template<typename _List, template<typename...> typename _Pred>
    using mp_min_element = /*see below*/;
  template<typename _List, typename _Quote>
    using mp_min_element_q = mp_min_element<_List, _Quote::template fn>;
  template<typename _List, template<typename...> typename _Pred>
    using mp_max_element = /*see below*/;
  template<typename _List, typename _Quote>
    using mp_max_element_q = mp_max_element<_List, _Quote::template fn>;

  template<typename _List, typename _Vp>
    using mp_find = /*see below*/;
  template<typename _List, template<typename...> typename _Pred>
    using mp_find_if = /*see below*/;
  template<typename _List, typename _Quote>
    using mp_find_if_q = mp_find_if<_List, _Quote::template fn>;

  template<typename _List> using mp_reverse = /*see below*/;

  template<typename _List, typename _Vp, template<typename...> typename _Fun>
    using mp_fold = /*see below*/;

  template<typename _List, typename _Vp, typename _Quote>
    using mp_fold_q = mp_fold<_List, _Vp, _Quote::template fn>;

  template<typename _List, typename _Vp, template<typename...> typename _Fun>
    using mp_reverse_fold = /*see below*/;

  template<typename _List, typename _Vp, typename _Quote>
    using mp_reverse_fold_q = mp_reverse_fold<_List, _Vp, _Quote::template fn>;

  template<typename _List>
    using mp_unique = /*see below*/;

  template<typename _List, template<typename...> typename _Pred>
    using mp_all_of = mp_bool<mp_count_if<_List, _Pred>::value == mp_size<_List>::value>;

  template<typename _List, typename _Quote>
    using mp_all_of_q = mp_all_of<_List, _Quote::template fn>;

  template<typename _List, template<typename...> typename _Pred>
    using mp_none_of = mp_bool<mp_count_if<_List, _Pred>::value == 0>;

  template<typename _List, typename _Quote> 
   using mp_none_of_q = mp_none_of<_List, _Quote::template fn>;

  template<typename _List, template<typename...> typename _Pred>
    using mp_any_of = mp_bool<mp_count_if<_List, _Pred>::value != 0>;

  template<typename _List, typename _Quote>
    using mp_any_of_q = mp_any_of<_List, _Quote::template fn>;

  template<typename _List, typename _Fun>
    constexpr _Fun
    mp_for_each(_Fun&& f);

  template<std::size_t _Num, typename _Fun>
    constexpr auto
    mp_with_index(std::size_t __index, _Fun&& __fun)
    -> decltype(declval<_Fun>()(declval<mp_size_t<0>>()));

  template<typename _Num, typename _Fun>
    constexpr auto
    mp_with_index(std::size_t __index, _Fun&& __fun)
    -> decltype(declval<_Fun>()(declval<mp_size_t<0>>()));

  // set operations

  template<typename _Set>
    using mp_is_set = /*see below*/;
  template<typename _Set, typename _Vp>
    using mp_set_contains = /*see below*/;
  template<typename _Set, typename... _Tp>
    using mp_set_push_back = /*see below*/;
  template<typename _Set, typename... _Tp>
    using mp_set_push_front = /*see below*/;

  // map operations

  namespace __detail
  {
    template<typename _Map>
      struct __is_map_impl
      { using type = mp_false; }

    //template<template<typename...> typename _Map, 
    //  struct __is_map_impl<_Map<_List<_K, _Tp...>...>>

    template<template<typename...> typename _Map, typename... _Tp, typename _Key>
      struct __map_find_impl<_Map<_Tp...>, _Key>
      {
	struct _Up
	: mp_identity<_Tp>...
	{ };

	template<template<typename...> typename _List, typename... _Up>
          static mp_identity<_List<_Key, _Up...>>
          __fun(mp_identity<_List<_Key, _Up...>>*);

	static mp_identity<void> __fun(...);

	using _Vp = decltype(__fun((_Up*)nullptr));

	using type = typename _Vp::type;
      };
  }

  template<typename _Map>
    using mp_is_map = /*see below*/;

  template<typename _Map, typename _Key>
    using mp_map_find = typename __detail::__map_find_impl<_Map, _Key>::type;

  template<typename _Map, typename _Key>
    using mp_map_contains = mp_not<std::is_same<mp_map_find<_Map, _Key>, void>>;

  template<typename _Map, typename _Tp>
    using mp_map_insert = mp_if<mp_map_contains<_Map, mp_first<_Tp>>, _Map, mp_push_back<_Map, _Tp>>;

  template<typename _Map, typename _Tp>
    using mp_map_replace = /*see below*/;

  template<typename _Map, typename _Tp, template<typename...> typename _Fun>
    using mp_map_update = /*see below*/;

  template<typename _Map, typename _Tp, typename _Quote>
    using mp_map_update_q = mp_map_update<_Map, _Tp, _Quote::template fn>;

  template<typename _Map, typename _Key>
    using mp_map_erase = /*see below*/;

  template<typename _Map>
    using mp_map_keys = mp_transform<mp_first, _Map>;

  // helper metafunctions

  namespace __detail
  {
    template<typename... _Tp>
      struct __and_impl;

    template<>
      struct __and_impl
      { using type = mp_false; };

    template<typename _Tp1, typename... _Tp>
      __and_impl<_Tp1, _Tp...>
      { using type = mp_bool<mp_to_bool<_Tp1> && __and_impl<_Tp...>::value>; };

    template<typename... _Tp>
      struct __or_impl;
  }

  template<typename... _Tp>
    using mp_and = /*see below*/;

  template<typename... _Tp>
    using mp_all = mp_bool<(static_cast<bool>(_Tp::value) && ...)>;

  template<typename... _Tp>
    using mp_or = /*see below*/;

  template<typename... _Tp>
    using mp_any = mp_bool<(static_cast<bool>(_Tp::value) || ...)>;

  template<typename... _Tp>
    using mp_same = /*see below*/;

  namespace _detail
  {
    template<typename... _Tp>
      struct __plus_impl<_Tp...>
      {
	static constexpr auto __val = (_Tp::value + ... + 0);
	using type
          = std::integral_constant<std::remove_const_t<decltype(__val)>, __val>;
      };
  }

  template<typename... _Tp>
    using mp_plus = typename __detail::__plus_impl<_Tp...>::type;

  template<typename _Tp1, typename _Tp2>
    using mp_less = mp_bool<_Tp1::value < _Tp2::value>;

  template<typename _Tp1, typename... _Tp>
    using mp_min = mp_min_element<mp_list<_Tp1, _Tp...>, mp_less>;

  template<typename _Tp1, typename... _Tp>
    using mp_max = mp_max_element<mp_list<_Tp1, _Tp...>, mp_less>;

  // bind

  template<std::size_t _Index>
    struct mp_arg;

  using _1 = mp_arg<0>;
  using _2 = mp_arg<1>;
  using _3 = mp_arg<2>;
  using _4 = mp_arg<3>;
  using _5 = mp_arg<4>;
  using _6 = mp_arg<5>;
  using _7 = mp_arg<6>;
  using _8 = mp_arg<7>;
  using _9 = mp_arg<8>;

  template<template<typename...> typename _Fun, typename... _Tp>
    struct mp_bind;
  template<typename _Quote, typename... _Tp>
    using mp_bind_q = mp_bind<_Quote::template fn, _Tp...>;

  template<template<typename...> typename _Fun, typename... _Tp>
    struct mp_bind_front;
  template<typename _Quote, typename... _Tp>
    using mp_bind_front_q = mp_bind_front<_Quote::template fn, _Tp...>;

  template<template<typename...> typename _Fun, typename... _Tp>
    struct mp_bind_back;
  template<typename _Quote, typename... _Tp>
    using mp_bind_back_q = mp_bind_back<_Quote::template fn, _Tp...>;

} // namespace std
