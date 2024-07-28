#import "Utils.hpp"

cocos2d::CCRect getScreenSafeAreaiOS() {
    cocos2d::CCRect safeAreaInsets;
    if (@available(iOS 11.0, *)) {
        UIWindow *keyWindow = [UIApplication sharedApplication].keyWindow;
        UIEdgeInsets insets = keyWindow.safeAreaInsets;
        
        // Convert from points to Cocos2d-x points (same in this case)
        safeAreaInsets.origin.x = insets.left;
        safeAreaInsets.origin.y = insets.top;
        safeAreaInsets.size.width = insets.right;
        safeAreaInsets.size.height = insets.bottom;
    } else {
        // Fallback on earlier versions
        safeAreaInsets = cocos2d::CCRect(0, 0, 0, 0);
    }
    return safeAreaInsets;
}