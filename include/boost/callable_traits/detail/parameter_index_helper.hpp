#ifndef BOOST_CLBL_TRTS_PARAMETER_INDEX_HELPER_HPP
#define BOOST_CLBL_TRTS_PARAMETER_INDEX_HELPER_HPP

#include <boost/callable_traits/detail/config.hpp>

namespace boost { namespace callable_traits { namespace detail {

template<std::size_t I, typename T, bool IgnoreThisPointer = false,
    bool AllowPlus1 = false, std::size_t Count = 0>
struct parameter_index_helper {

    using error_t = error_type<T>;

    using args_tuple = std::conditional_t<IgnoreThisPointer,
        typename detail::traits<T>::non_invoke_arg_types,
        typename detail::traits<T>::arg_types>;

    static constexpr bool has_parameter_list =
        !std::is_same<args_tuple, invalid_type>::value
        && !std::is_same<args_tuple, reference_error>::value;

    using temp_tuple = std::conditional_t<has_parameter_list,
        args_tuple, std::tuple<error_t>>;

    static constexpr std::size_t parameter_list_size =
        std::tuple_size<temp_tuple>::value;

    static constexpr bool is_out_of_range = has_parameter_list &&
        I >= parameter_list_size + static_cast<std::size_t>(AllowPlus1);

    static constexpr bool is_count_out_of_range = has_parameter_list &&
        I + Count > parameter_list_size + static_cast<std::size_t>(AllowPlus1);

    static constexpr std::size_t index =
        has_parameter_list && !is_out_of_range ? I : 0;

    static constexpr std::size_t count =
        has_parameter_list && !is_count_out_of_range ? Count : 0;

    using permissive_tuple = std::conditional_v<
        has_parameter_list && !is_out_of_range,
        args_tuple, std::tuple<error_t>>;

    using permissive_function = std::conditional_v<
        has_parameter_list && !is_out_of_range,
        T, error_t(error_t)>;
};

}}} // namespace boost::callable_traits::detail

#endif // #ifndef BOOST_CLBL_TRTS_PARAMETER_INDEX_HELPER_HPP
