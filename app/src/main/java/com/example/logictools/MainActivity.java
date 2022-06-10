package com.example.logictools;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.text.Editable;
import android.view.View;
import android.view.Window;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import com.example.logictools.databinding.ActivityMainBinding;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'logictools' library on application startup.
    static {
        System.loadLibrary("logictools");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // UI elements variables
        Button clearButton = binding.clearButton;
        Button solveButton = binding.solveButton;
        Button randomButton = binding.randomButton;
        Button andButton = binding.conjunctionButton;
        Button orButton = binding.disjunctionButton;
        Button strongOrButton = binding.strongDisButton;
        Button ifButton = binding.implicationButton;
        Button notButton = binding.notButton;
        Button biconditionalButton = binding.equalButton;
        EditText et_formulaField = binding.formulaEditText;
        ListView truthTable = binding.truthTableListView;


        /*
        FUNCTIONALITY
        */


        /*
         * "AND" button
         */

        andButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Editable temp = et_formulaField.getText();
                int pos = et_formulaField.getSelectionStart();
                temp.insert(pos, " & ");
                et_formulaField.setText(temp);
                et_formulaField.setSelection(pos + 3);
            }
        });


        /*
         * "NOT" button
         */

        notButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Editable temp = et_formulaField.getText();
                int pos = et_formulaField.getSelectionStart();
                temp.insert(pos, "~");
                et_formulaField.setText(temp);
                et_formulaField.setSelection(pos + 1);
            }
        });


        /*
         * "EQUAL" button
         */

        biconditionalButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Editable temp = et_formulaField.getText();
                int pos = et_formulaField.getSelectionStart();
                temp.insert(pos, " = ");
                et_formulaField.setText(temp);
                et_formulaField.setSelection(pos + 3);
            }
        });


        /*
         * "OR" button
         */

        orButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Editable temp = et_formulaField.getText();
                int pos = et_formulaField.getSelectionStart();
                temp.insert(pos, " | ");
                et_formulaField.setText(temp);
                et_formulaField.setSelection(pos + 3);
            }
        });


        /*
         * "STRONG OR" button
         */

        strongOrButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Editable temp = et_formulaField.getText();
                int pos = et_formulaField.getSelectionStart();
                temp.insert(pos, " ! ");
                et_formulaField.setText(temp);
                et_formulaField.setSelection(pos + 3);
            }
        });


        /*
         * "IF" button
         */

        ifButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                {
                    Editable temp = et_formulaField.getText();
                    int pos = et_formulaField.getSelectionStart();
                    temp.insert(pos, " -> ");
                    et_formulaField.setText(temp);
                    et_formulaField.setSelection(pos + 4);
                }
            }
        });


        /*
         * "CLEAR" button
         */

        clearButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                et_formulaField.setText("");
                truthTable.setAdapter(null);
                //tv_totalContacts.setText(getString(R.string.counter_messages, String.valueOf(0)));
            }
        });


        /*
         * "SOLVE" button
         */

        solveButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                truthTable.setAdapter(null);

                // Get input string
                String strRequest = et_formulaField.getText().toString();

                // Send request to c++, then store answer
                ArrayList<String> truthTableRows = getTruthTableRows(strRequest);

                // Create new adapter
                ArrayAdapter<String> adapter = new ArrayAdapter<String>(getBaseContext(), R.layout.custom_list_item, truthTableRows);

                // Refresh info
                truthTable.setAdapter(adapter);
            }
        });


        /*
         * "RANDOM" button
         */

        randomButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                // Refresh editText
                truthTable.setAdapter(null);

                // Show new formula
                et_formulaField.setText(getRandomFormula());
            }
        });
    }


    // Auxiliary methods
    ArrayList<String> getTruthTableRows(String request)
    {
        // Result array
        ArrayList<String> truthTableRows = new ArrayList<String>();

        // Create JSON Object
        JSONObject jRequest = new JSONObject();

        String result = "None";


        // Java sends JSON to C++, then answer is stored in 'result'
        try {
            jRequest.put("Formula", request);
            result = getTruthTable(jRequest.toString());
        } catch (JSONException e) {
            e.printStackTrace();
            truthTableRows.add(result);
        }


        // Translate JSON string from C++
        try {
            JSONArray truthTable = (new JSONObject(result)).getJSONArray("truthTable");

            for (int i = 0; i < truthTable.length(); i++)
            {
                JSONObject truthTableRow = truthTable.getJSONObject(i);

                String rowValues = truthTableRow.getString("Values");
                String rowResult = truthTableRow.getString("Result");
                truthTableRows.add(rowValues + "\n" + "==> " + rowResult);
            }

        }
        catch (JSONException e)
        {
            truthTableRows.add(result);
        }

        return truthTableRows;
    }


    String getRandomFormula()
    {
        JSONObject jRequest;

        try {
            jRequest = new JSONObject(sendRandomFormula());
            return jRequest.getString("Formula");
        }
        catch (JSONException e)
        {
            return "TRY AGAIN";
        }

    }


    // Native methods
    public native String getTruthTable(String jsonString);
    public native String sendRandomFormula();
}