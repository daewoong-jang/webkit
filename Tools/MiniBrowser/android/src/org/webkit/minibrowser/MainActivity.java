package org.webkit.minibrowser;

import org.webkit.WebView;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;

public class MainActivity extends Activity {

    private WebView mWebView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mWebView = new WebView(this);
        setContentView(mWebView);
        Intent intent = getIntent();
        mWebView.loadUrl(intent.hasExtra("arg1") ? intent.getCharSequenceExtra(
                "arg1").toString() : "http://webkit.org");
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onBackPressed() {
        if (mWebView != null && mWebView.canGoBack()) {
            mWebView.goBack();
            return;
        }
        super.onBackPressed();
    }

    @Override
    public void setContentView(View view) {
        super.setContentView(R.layout.activity_main);
        ViewGroup mainView = (ViewGroup) this.findViewById(R.id.main);
        mainView.addView(mWebView);
        initializeUI();
    }

    private void initializeUI() {
        ImageButton backButton = (ImageButton) this
                .findViewById(R.id.backButton);
        backButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mWebView == null)
                    return;

                if (mWebView.canGoBack())
                    mWebView.goBack();
            }
        });
        ImageButton forwardButton = (ImageButton) this
                .findViewById(R.id.forwardButton);
        forwardButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mWebView == null)
                    return;

                if (mWebView.canGoForward())
                    mWebView.goForward();
            }
        });
        EditText editUrl = (EditText) this.findViewById(R.id.editUrl);
        editUrl.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId,
                    KeyEvent event) {
                boolean handled = false;
                if (mWebView != null && actionId == EditorInfo.IME_ACTION_GO) {
                    String url = v.getText().toString();
                    if (!url.contains(":")) {
                        url = "http://" + url;
                        v.setText(url);
                    }
                    mWebView.loadUrl(url);
                    handled = true;
                }
                InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(v.getWindowToken(), 0);
                v.setFocusable(false);
                return handled;
            }
        });
        editUrl.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                v.setFocusableInTouchMode(true);
                return false;
            }
        });
        editUrl.setFocusable(false);
    }
}
