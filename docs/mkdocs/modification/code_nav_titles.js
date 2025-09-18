function all_lowercase(str) {
    return str === str.toLowerCase() && str !== str.toUpperCase();
}

function has_parent_with_class(element, class_name) {
    // If we are here we didn't find the searched class in any parents node
    if (!element.parentNode) return false;

    // If the current node has the class return true, otherwise we will search it in the parent node
    if (element.className.split(' ').indexOf(class_name) >= 0) return true;
    return has_parent_with_class(element.parentNode, class_name);
}

{
    let nav_titles = [...document.getElementsByClassName('md-ellipsis')];

    for (let title of nav_titles) {
        const is_lowercase_title = all_lowercase(title.innerHTML);
        const is_not_a_header    = !has_parent_with_class(title, 'md-header__topic')
        // without second condition the topic header displayed when we scroll down would
        // also be affected which causes it to be weirdly misaligned
        
        if (is_lowercase_title && is_not_a_header) {
            title.classList.add('md-typeset');
            title.innerHTML = '<code class="code-nav-title">' + title.innerHTML + '</code>';
            // custom class is needed for color-on-hover similar to the regular sections
        }
    }
}
